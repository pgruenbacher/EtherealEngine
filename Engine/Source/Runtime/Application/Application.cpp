#include "Application.h"
#include "Core/core.h"
#include "Graphics/graphics.h"
#include "../System/Singleton.h"
#include "../System/Timer.h"
#include "../System/FileSystem.h"
#include "../System/MessageBox.h"
#include "../Threading/ThreadPool.h"
#include "../Assets/AssetManager.h"
#include "../Assets/AssetReader.h"
#include "../Assets/AssetWriter.h"
#include "../Rendering/RenderView.h"
#include "../Rendering/FrameBuffer.h"
#include "../Rendering/Debug/DebugDraw.h"
#include "../Rendering/RenderWindow.h"
#include "../Input/InputContext.h"
#include "../Ecs/World.h"
#include "../Ecs/Systems/TransformSystem.h"
#include "../Ecs/Systems/CameraSystem.h"
#include "../Ecs/Systems/RenderingSystem.h"

struct GfxCallback : public gfx::CallbackI
{
	virtual ~GfxCallback()
	{
	}

	virtual void traceVargs(const char* _filePath, std::uint16_t _line, const char* _format, std::va_list _argList) BX_OVERRIDE
	{		
		auto logger = logging::get("Log");
		logger->trace() << string_utils::format(_format, _argList).c_str();
	}

	virtual void fatal(gfx::Fatal::Enum _code, const char* _str) BX_OVERRIDE
	{
		auto logger = logging::get("Log");

		logger->error() << _str;
	}

	virtual uint32_t cacheReadSize(uint64_t /*_id*/) BX_OVERRIDE
	{
		return 0;
	}

	virtual bool cacheRead(uint64_t /*_id*/, void* /*_data*/, uint32_t /*_size*/) BX_OVERRIDE
	{
		return false;
	}

	virtual void cacheWrite(uint64_t /*_id*/, const void* /*_data*/, uint32_t /*_size*/) BX_OVERRIDE
	{
	}

	virtual void screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data, uint32_t _size, bool _yflip) BX_OVERRIDE
	{
		BX_UNUSED(_filePath, _width, _height, _pitch, _data, _size, _yflip);
	}

	virtual void captureBegin(uint32_t /*_width*/, uint32_t /*_height*/, uint32_t /*_pitch*/, gfx::TextureFormat::Enum /*_format*/, bool /*_yflip*/) BX_OVERRIDE
	{
		BX_TRACE("Warning: using capture without callback (a.k.a. pointless).");
	}

	virtual void captureEnd() BX_OVERRIDE
	{
	}

	virtual void captureFrame(const void* /*_data*/, uint32_t /*_size*/) BX_OVERRIDE
	{
	}

};
static GfxCallback sGfxCallback;
//-----------------------------------------------------------------------------
//  Name : Application () (Constructor)
/// <summary>
/// Application Class Constructor
/// </summary>
//-----------------------------------------------------------------------------
Application::Application()
{
    // Set members to sensible defaults
	mWorld = std::make_unique<World>();
	mAssetManagerEx = std::make_unique<AssetManager>();
	mTimer = std::make_unique<Timer>();
	mThreadPool = std::make_unique<ThreadPool>();
	mActionMapper = std::make_unique<ActionMapper>();
}

//-----------------------------------------------------------------------------
//  Name : ~Application () (Destructor)
/// <summary>
/// Application Class Destructor
/// </summary>
//-----------------------------------------------------------------------------
Application::~Application()
{
}

InputContext& Application::getInput()
{
	return getWindow().getInput();
}

RenderWindow& Application::getWindow()
{
	Expects(mWindow) return *mWindow.get();
}

RenderWindow& Application::getMainWindow()
{
	Expects(!mWindows.empty()) return *(mWindows[0].get());
}

//-----------------------------------------------------------------------------
//  Name : initInstance ()
/// <summary>
/// Initializes the entire engine here.
/// </summary>
//-----------------------------------------------------------------------------
bool Application::initInstance(const std::string& rootDataDir, const std::string& strCmdLine )
{
	setRootDataPath(rootDataDir);

	fs::addPathProtocol("sys", rootDataDir + "../../Engine_Data/");

	// Create and initialize the logging system
	if (!initLogging()) { shutDown(); return false; }

	// Create and initialize the input mappings
	if (!initInputMappings()) { shutDown(); return false; }

    // Create and initialize the primary display device
    if (!initDisplay()) { shutDown(); return false; }

	// Create and initialize the logging system
	if (!initAssetManager()) { shutDown(); return false; }

	// Create and initialize the systems
	if (!initSystems()) { shutDown(); return false; }

    // Initialize the application systems
    if (!initApplication()) { shutDown(); return false; }

    // Success!
    return true;
}

//-----------------------------------------------------------------------------
//  Name : setRootDataPath ()
/// <summary>
/// Set the path that represents the root from which all data will be loaded
/// or leave blank to use the process current directory. The information
/// provided via this method only takes effect during a subsequent call to 
/// 'initInstance()' and cannot be used to make changes after the fact.
/// </summary>
//-----------------------------------------------------------------------------
void Application::setRootDataPath(const std::string & path)
{
	mRootDataDir = path;

	// Initialize file system and search for all valid data packages
	fs::setRootDirectory(mRootDataDir);
}

//-----------------------------------------------------------------------------
//  Name : setCopyrightData ()
/// <summary>
/// Set the application copyright information as it should be displayed to the
/// user where applicable.
/// </summary>
//-----------------------------------------------------------------------------
void Application::setCopyrightData( const std::string & copyright )
{
    mCopyright = copyright;
}

//-----------------------------------------------------------------------------
//  Name : setVersionData ()
/// <summary>
/// Set the application version information as it should be displayed to the
/// user where applicable.
/// </summary>
//-----------------------------------------------------------------------------
void Application::setVersionData( const std::string & version )
{
    mVersion = version;
}


//-----------------------------------------------------------------------------
//  Name : createMainWindow ()
/// <summary>
/// Set the application main window.
/// </summary>
//-----------------------------------------------------------------------------
bool Application::registerMainWindow(RenderWindow& window)
{
	gfx::PlatformData pd;
	pd.ndt = nullptr;
	pd.nwh = window.getSystemHandle();
	pd.context = nullptr;
	pd.backBuffer = nullptr;
	pd.backBufferDS = nullptr;
	gfx::setPlatformData(pd);

	if (!gfx::init(gfx::RendererType::Count, 0, 0, &sGfxCallback))
		return false;

	auto onResized = [this](RenderWindow& wnd, const uSize& size)
	{
		std::uint32_t flags = 0;
		if (mVsync)
			flags |= BGFX_RESET_VSYNC;
		gfx::reset(size.width, size.height, flags);
	};

	auto onClosed = [this](RenderWindow& wnd)
	{
		mRunning = false;
	};

	
	auto size = window.getSize();

	onResized(window, size);
	window.onResized.addListener(onResized);
	window.onClosed.addListener(onClosed);
	
	return true;
}


void Application::registerWindow(std::shared_ptr<RenderWindow> window)
{
	auto onClosed = [this](RenderWindow& wnd)
	{
		mWindows.erase(std::remove_if(std::begin(mWindows), std::end(mWindows),
			[&wnd](std::shared_ptr<RenderWindow>& other)
		{
			return &wnd == other.get();
		}), std::end(mWindows));
	};
	
	window->getInput().setActionMapper(mActionMapper.get());
	window->prepareView();
	window->onClosed.addListener(onClosed);
	mWindows.emplace_back(window);
}

std::shared_ptr<RenderWindow> Application::createMainWindow()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	return std::make_shared<RenderWindow>(sf::VideoMode{ 1280, 720, desktop.bitsPerPixel }, "App", sf::Style::Default);
}

bool Application::initLogging()
{
	auto logger = logging::create("Log",
	{
		std::make_shared<logging::sinks::platform_sink_mt>(),
		std::make_shared<logging::sinks::daily_file_sink_mt>("Log", "log", 23, 59),
	});
	//logger->set_level(logging::level::trace);
	if (!logger)
	{
		misc::messageBox(
			"Failed to initialize the logging system!\n"
			"Application will terminate now.",
			"Initialization Error",
			misc::Style::Error,
			misc::Buttons::OK);

		return false;
	}

	logger->info() << "Logging System Initialized!";

	return true;
}


bool Application::initAssetManager()
{
	auto& manager = getAssetManager();
	{
		auto storage = manager.add<Shader>();
		storage->loadFromFile = AssetReader::loadShaderFromFile;
		storage->loadFromMemory = AssetReader::loadShaderFromMemory;
		
		switch (gfx::getRendererType())
		{
		case gfx::RendererType::Direct3D9:
			storage->subdir = "/runtime/dx9/";
			break;
		case gfx::RendererType::Direct3D11:
		case gfx::RendererType::Direct3D12:
			storage->subdir = "/runtime/dx11/";
			break;

		case gfx::RendererType::OpenGL:
			storage->subdir = "/runtime/glsl/";
			break;

		case gfx::RendererType::Metal:
			storage->subdir = "/runtime/metal/";
			break;

		case gfx::RendererType::OpenGLES:
			storage->subdir = "/runtime/gles/";
			break;

		default:
			break;
		}

	}
	{
		auto storage = manager.add<Texture>();
		storage->subdir = "/runtime/";
		storage->loadFromFile = AssetReader::loadTextureFromFile;
		//storage->loadFromMemory = AssetReader::loadTextureFromMemory;
	}
	{
		auto storage = manager.add<Mesh>();
		storage->subdir = "/runtime/";
		storage->loadFromFile = AssetReader::loadMeshFromFile;
		//storage->loadFromMemory = AssetReader::loadMeshFromMemory;
	}
	{
		auto storage = manager.add<Material>();
		storage->subdir = "/runtime/";
		storage->loadFromFile = AssetReader::loadMaterialFromFile;
		storage->saveToFile = AssetWriter::saveMaterialToFile;
		//storage->loadFromMemory = AssetReader::loadMaterialFromMemory;
	}

	return true;
}

bool Application::initInputMappings()
{
	auto logger = logging::get("Log");

	return true;
}

//-----------------------------------------------------------------------------
//  Name : initDisplay ()
/// <summary>
/// Initialize the display driver, create device window, load
/// render configuration etc.
/// </summary>
//-----------------------------------------------------------------------------
bool Application::initDisplay()
{
	auto logger = logging::get("Log");

	auto window = createMainWindow();

	if (!registerMainWindow(*window))
	{
		// Write debug info
		logger->critical() << "Failed to initialize render driver.";

		// Failure to initialize!
		misc::messageBox(
			"Failed to initialize rendering hardware. The application must now exit.",
			"Fatal Error",
			misc::Style::Error,
			misc::Buttons::OK
		);

		return false;
	}

	registerWindow(window);

    // Success!!
    return true;
}

bool Application::initSystems()
{
	auto& world = getWorld();
	world.systems.add<TransformSystem>();
	world.systems.add<CameraSystem>();
	world.systems.add<RenderingSystem>();
	// Success!!
	return true;
}
//-----------------------------------------------------------------------------
//  Name : initApplication ()
/// <summary>
/// Initialize all required aspects of the application ready for us to begin.
/// This includes setting up all required states that the application may enter
/// etc.
/// </summary>
//-----------------------------------------------------------------------------
bool Application::initApplication( )
{
	auto& world = getWorld();

	world.systems.configure();

	ddInit(true);
	// Success!!
    return true;
}

//-----------------------------------------------------------------------------
//  Name : begin ()
/// <summary>
/// Signals the beginning of the physical post-initialization stage.
/// From here on, the game engine has control over processing.
/// </summary>
//-----------------------------------------------------------------------------
int Application::begin( )
{
    // Get access to required systems
	auto logger = logging::get("Log");
    // 'Ping' the timer for the first frame
	mTimer->tick();
 
     // Write debug info
	logger->info() << "Entered main application processing loop.";

    // Start main loop
    while(mRunning)
    {
        // Advance and render frame.
		frameAdvance( );
    } // Until quit message is received

    return 0;
}


//-----------------------------------------------------------------------------
//  Name : frameAdvance ()
/// <summary>
/// Process the next application frame.
/// </summary>
//-----------------------------------------------------------------------------
bool Application::frameAdvance( bool bRunSimulation /* = true */ )
{
    // Advance Game Frame.
    if ( frameBegin( bRunSimulation ) )
    {
		// Did we receive a message, or are we idling ?
		auto windows = mWindows;
		for (auto sharedWindow : windows)
		{
			auto& window = *sharedWindow;
			mWindow = sharedWindow;
			processWindow(window);
			mWindow.reset();
		}

		frameEnd();
        return true;
    
    } // End if frame rendering can commence

    return false;
}

//-----------------------------------------------------------------------------
//  Name : shutDown ()
/// <summary>
/// Shuts down the game engine, and frees up all resources.
/// Note : You must explicitly call this method PRIOR to cleaning up the
/// engine with a call to EngineCleanup()
/// </summary>
//-----------------------------------------------------------------------------
bool Application::shutDown()
{
	ddShutdown();
	auto logger = logging::get("Log");
	logger->info() << "Shutting down main application.";

	mWindows.clear();
	mWorld.reset();
	mThreadPool.reset();
	mAssetManagerEx.reset();
	mTimer.reset();

	gfx::shutdown();

    // Shutdown Success
    return true;
}

void Application::quit()
{
	mRunning = false;
}



//-----------------------------------------------------------------------------
//  Name : frameBegin () (Protected, Virtual)
/// <summary>
/// Called to signal that we are just about to commence rendering of the
/// current frame.
/// </summary>
//-----------------------------------------------------------------------------
bool Application::frameBegin( bool bRunSimulation /* = true */ )
{
	// Allowing simulation to run?
    if ( bRunSimulation )
    {
        // In order to help avoid input lag when VSync is enabled, it is sometimes
        // recommended that the application cap its frame-rate manually where possible.
        // This helps to ensure that there is a consistent delay between input polls
        // rather than the variable time when the hardware is waiting to draw.
        auto fCap = mMaximumFPS;
		bool vsync = mVsync;
		if (vsync)
		{
		    float fSmoothedCap = mMaximumSmoothedFPS;
		    if ( fSmoothedCap < fCap || fCap == 0 )
			{
		        fCap = fSmoothedCap;
			}

		} // End if cap frame rate

        // Advance timer.
		mTimer->tick( fCap );

    } // End if bRunSimulation
    else
    {
        // Do not advance simulation time.
        auto fOldSimulationSpeed = mTimer->getSimulationSpeed();
        mTimer->setSimulationSpeed( 0.0 );
        mTimer->tick();
        mTimer->setSimulationSpeed( fOldSimulationSpeed );
    
    } // End if !bRunSimulation

	// Increment the frame counter
	mTimer->incrementFrameCounter( );

	mThreadPool->poll();

    // Success, continue on to render
    return true;
}

void Application::processWindow(RenderWindow& window)
{
	if (window.isOpen())
	{
		frameWindowBegin(window);
		frameWindowUpdate(window);
		frameWindowRender(window);
		frameWindowEnd(window);
	}
}

void Application::frameWindowBegin(RenderWindow& window)
{

	window.frameBegin();

	sf::Event e;
	while (window.pollEvent(e))
	{

	}

	if (window.hasFocus())
		getWorld().systems.frameBegin(static_cast<float>(mTimer->getDeltaTime()));
}

void Application::frameWindowUpdate(RenderWindow& window)
{	
	window.frameUpdate(static_cast<float>(mTimer->getDeltaTime()));

	if(window.hasFocus())
		getWorld().systems.frameUpdate(static_cast<float>(mTimer->getDeltaTime()));
}

//-----------------------------------------------------------------------------
//  Name : onFrame () (Protected, Virtual)
/// <summary>
/// Actually performs the default rendering of the current frame.
/// </summary>
//-----------------------------------------------------------------------------
void Application::frameWindowRender(RenderWindow& window)
{
	window.frameRender();

	if (window.hasFocus())
		getWorld().systems.frameRender(static_cast<float>(mTimer->getDeltaTime()));
}

//-----------------------------------------------------------------------------
//  Name : frameWindowEnd () (Protected, Virtual)
/// <summary>
/// Called to signal that we have finished rendering the current frame.
/// </summary>
//-----------------------------------------------------------------------------
void Application::frameWindowEnd(RenderWindow& window)
{
	window.frameEnd();

	if (window.hasFocus())
		getWorld().systems.frameEnd(static_cast<float>(mTimer->getDeltaTime()));
	
}

//-----------------------------------------------------------------------------
//  Name : frameEnd () (Protected, Virtual)
/// <summary>
/// Called to signal that we have finished rendering the current frame.
/// </summary>
//-----------------------------------------------------------------------------
void Application::frameEnd()
{
	Expects(RenderView::getStack().empty());

	RenderView::clearStack();
	// Advance to next frame. Rendering thread will be kicked to
	// process submitted rendering primitives.
	mRenderFrame = gfx::frame();
	
}