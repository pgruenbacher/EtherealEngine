#include "TransformComponent.h"
#include <algorithm>

#include "../../Application/Application.h"
#include "../../System/Singleton.h"
#include "../World.h"
#include "Core/logging/logging.h"
ComponentHandle<TransformComponent> createFromComponent(ComponentHandle<TransformComponent> component)
{
	if (!component.expired())
	{
		auto& app = Singleton<Application>::getInstance();
		auto& world = app.getWorld();
		auto entity = world.entities.create_from_copy(component.lock()->getEntity());
		return entity.component<TransformComponent>();
	}
	logging::get("Log")->error("trying to clone a null component");
	return ComponentHandle<TransformComponent>();
}
//-----------------------------------------------------------------------------
//  Name : TransformComponent () (Constructor)
/// <TransformComponent>
/// Entity Class Constructor.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent::TransformComponent()
{
}

TransformComponent::TransformComponent(const TransformComponent& rhs)
{
	//mParent = rhs.mParent;
	mChildren.reserve(rhs.mChildren.size());
	for (auto& rhsChild : rhs.mChildren)
	{
		mChildren.push_back(createFromComponent(rhsChild));
	}

	mLocalTransform = rhs.mLocalTransform;
	mWorldTransform = rhs.mWorldTransform;
	mHierarchyLevel = rhs.mHierarchyLevel;
	mSlowParenting = rhs.mSlowParenting;
	mSlowParentingSpeed = rhs.mSlowParentingSpeed;
}


void TransformComponent::onEntitySet()
{
// 	if (!mParent.expired())
// 	{
// 		mParent.lock()->attachChild(makeHandle());
// 	}

	for (auto& child : mChildren)
	{
		child.lock()->mParent = makeHandle();
	}
}
//-----------------------------------------------------------------------------
//  Name : TransformComponent () (Destructor)
/// <summary>
/// TransformComponent Class Destructor.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent::~TransformComponent()
{
	if (!mParent.expired())
	{
		if(getEntity())
			mParent.lock()->removeChild(makeHandle());
	}
	for (auto& child : mChildren)
	{
		if(!child.expired())
			child.lock()->getEntity().destroy();
	}

}

//-----------------------------------------------------------------------------
//  Name : move()
/// <summary>
/// Move the current position of the node by the specified amount.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::move(const math::vec3 & amount)
{
	math::vec3 vNewPos = getPosition();
	vNewPos += getXAxis() * amount.x;
	vNewPos += getYAxis() * amount.y;
	vNewPos += getZAxis() * amount.z;

	// Pass through to setPosition so that any derived classes need not
	// override the 'move' method in order to catch this position change.
	setPosition(vNewPos);
	return *this;
}

//-----------------------------------------------------------------------------
//  Name : moveLocal()
/// <summary>
/// Move the current position of the node by the specified amount relative to
/// its own local axes.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::moveLocal(const math::vec3 & amount)
{
	math::vec3 vNewPos = getLocalPosition();
	vNewPos += getLocalXAxis() * amount.x;
	vNewPos += getLocalYAxis() * amount.y;
	vNewPos += getLocalZAxis() * amount.z;

	// Pass through to setPosition so that any derived classes need not
	// override the 'move' method in order to catch this position change.
	setLocalPosition(vNewPos);
	return *this;
}


//-----------------------------------------------------------------------------
//  Name : setPosition()
/// <summary>
/// Set the current world space position of the node.
/// Note : This bypasses the physics system, so should really only be used
/// for initialization purposes.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::setLocalPosition(const math::vec3 & position)
{
	// Set new cell relative position
	mLocalTransform.setPosition(position);
	setLocalTransform(mLocalTransform);
	return *this;
}

//-----------------------------------------------------------------------------
//  Name : setPosition()
/// <summary>
/// Set the current world space position of the node.
/// Note : This bypasses the physics system, so should really only be used
/// for initialization purposes.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::setPosition(const math::vec3 & position)
{
	// Rotate a copy of the current math::transform.
	math::transform m = getTransform();
	m.setPosition(position);

	if (!mParent.expired())
	{
		math::transform invParentTransform = math::inverse(mParent.lock()->getTransform());
		m = invParentTransform * m;
	}

	setLocalTransform(m);
	return *this;
}


//-----------------------------------------------------------------------------
//  Name : getScale()
/// <summary>
/// Retrieve the current scale of the node along its world space axes.
/// </summary>
//-----------------------------------------------------------------------------
math::vec3 TransformComponent::getLocalScale()
{
	return mLocalTransform.getScale();
}

//-----------------------------------------------------------------------------
//  Name : getPosition()
/// <summary>
/// Retrieve the current world space position of the node.
/// Optionally, caller can retrieve the position at the pivot point or 
/// otherwise.
/// </summary>
//-----------------------------------------------------------------------------
const math::vec3 & TransformComponent::getLocalPosition()
{
	return mLocalTransform.getPosition();
}

//-----------------------------------------------------------------------------
//  Name : getOrientation()
/// <summary>
/// Retrieve the current orientation of the node in world space.
/// Optionally, caller can retrieve the orientation at the pivot point or 
/// otherwise.
/// </summary>
//-----------------------------------------------------------------------------
math::quat TransformComponent::getLocalRotation()
{
	return mLocalTransform.getRotation();
}

//-----------------------------------------------------------------------------
//  Name : getXAxis()
/// <summary>
/// Retrieve the current orientation of the node in world space.
/// Optionally, caller can retrieve the orientation at the pivot point or 
/// otherwise.
/// </summary>
//-----------------------------------------------------------------------------
math::vec3 TransformComponent::getLocalXAxis()
{
	return mLocalTransform.xUnitAxis();
}

//-----------------------------------------------------------------------------
//  Name : getYAxis()
/// <summary>
/// Retrieve the current orientation of the node in world space.
/// Optionally, caller can retrieve the orientation at the pivot point or 
/// otherwise.
/// </summary>
//-----------------------------------------------------------------------------
math::vec3 TransformComponent::getLocalYAxis()
{
	return mLocalTransform.yUnitAxis();

}

//-----------------------------------------------------------------------------
//  Name : getZAxis()
/// <summary>
/// Retrieve the current orientation of the node in world space.
/// Optionally, caller can retrieve the orientation at the pivot point or 
/// otherwise.
/// </summary>
//-----------------------------------------------------------------------------
math::vec3 TransformComponent::getLocalZAxis()
{
	return mLocalTransform.zUnitAxis();

}

//-----------------------------------------------------------------------------
//  Name : getPosition()
/// <summary>
/// Retrieve the current world space position of the node.
/// </summary>
//-----------------------------------------------------------------------------
const math::vec3 & TransformComponent::getPosition()
{
	return getTransform().getPosition();
}

//-----------------------------------------------------------------------------
//  Name : getOrientation()
/// <summary>
/// Retrieve the current orientation of the node in world space.
/// </summary>
//-----------------------------------------------------------------------------
math::quat TransformComponent::getRotation()
{
	return getTransform().getRotation();
}

//-----------------------------------------------------------------------------
//  Name : getXAxis()
/// <summary>
/// Retrieve the current orientation of the node in world space.
/// </summary>
//-----------------------------------------------------------------------------
math::vec3 TransformComponent::getXAxis()
{
	return getTransform().xUnitAxis();
}

//-----------------------------------------------------------------------------
//  Name : getYAxis()
/// <summary>
/// Retrieve the current orientation of the node in world space.
/// </summary>
//-----------------------------------------------------------------------------
math::vec3 TransformComponent::getYAxis()
{
	return getTransform().yUnitAxis();
}

//-----------------------------------------------------------------------------
//  Name : getZAxis()
/// <summary>
/// Retrieve the current orientation of the node in world space.
/// </summary>
//-----------------------------------------------------------------------------
math::vec3 TransformComponent::getZAxis()
{
	return getTransform().zUnitAxis();
}

//-----------------------------------------------------------------------------
//  Name : getScale()
/// <summary>
/// Retrieve the current scale of the node along its world space axes.
/// </summary>
//-----------------------------------------------------------------------------
math::vec3 TransformComponent::getScale()
{
	return getTransform().getScale();
}

//-----------------------------------------------------------------------------
//  Name : getTransform()
/// <summary>
/// Retrieve the node's current world math::transform at its pivot.
/// </summary>
//-----------------------------------------------------------------------------
const math::transform & TransformComponent::getTransform()
{
	resolveTransform();
	return mWorldTransform;
}

//-----------------------------------------------------------------------------
//  Name : getLocalTransform()
/// <summary>
/// Retrieve the node's parent relative 'local' transformation matrix.
/// </summary>
//-----------------------------------------------------------------------------
const math::transform & TransformComponent::getLocalTransform() const
{
	// Return reference to our internal matrix
	return mLocalTransform;
}


//-----------------------------------------------------------------------------
//  Name : lookAt()
/// <summary>
/// Instruct the object to look at the specified point.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::lookAt(float x, float y, float z)
{
	//TODO("General", "These lookAt methods need to consider the pivot and the currently applied math::transform method!!!");
	lookAt(getPosition(), math::vec3(x, y, z));
	return *this;
}

//-----------------------------------------------------------------------------
//  Name : lookAt()
/// <summary>
/// Instruct the object to look at the specified point.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::lookAt(const math::vec3 & point)
{
	lookAt(getPosition(), point);
	return *this;
}

//-----------------------------------------------------------------------------
//  Name : lookAt()
/// <summary>
/// Instruct the object to look at the specified point.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::lookAt(const math::vec3 & eye, const math::vec3 & at)
{
	math::transform m;
	m.lookAt(eye, at);

	// Update the component position / orientation through the common base method.
	math::vec3 translation;
	math::quat orientation;

	if (m.decompose(orientation, translation))
	{
		setRotation(orientation);
		setPosition(translation);
	}
	return *this;
}

//-----------------------------------------------------------------------------
//  Name : lookAt()
/// <summary>
/// Instruct the object to look at the specified point, aligned to a
/// prefered / specific up axis.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::lookAt(const math::vec3 & eye, const math::vec3 & at, const math::vec3 & up)
{
	math::transform m;
	m.lookAt(eye, at, up);

	// Update the component position / orientation through the common base method.
	math::vec3 translation;
	math::quat orientation;

	if (m.decompose(orientation, translation))
	{
		setRotation(orientation);
		setPosition(translation);
	}
	return *this;
}


//-----------------------------------------------------------------------------
//  Name : rotateLocal ()
/// <summary>
/// Rotate the node around its own local axes.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::rotateLocal(float x, float y, float z)
{
	// Do nothing if rotation is disallowed.
	if (!canRotate())
		return *this;

	// No-op?
	if (!x && !y && !z)
		return *this;

	mLocalTransform.rotateLocal(math::radians(x), math::radians(y), math::radians(z));
	setLocalTransform(mLocalTransform);
	return *this;
}

//-----------------------------------------------------------------------------
//  Name : rotateAxis ()
/// <summary>
/// Rotate the node around a specified axis
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::rotateAxis(float degrees, const math::vec3 & axis)
{
	// No - op?
	if (!degrees)
		return *this;

	// If rotation is disallowed, only process position change. Otherwise
	// perform full rotation.
	if (!canRotate())
	{
		// Scale the position, but do not allow axes to scale.
		math::vec3 vPos = getPosition();
		math::transform t;
		t.rotateAxis(math::radians(degrees), axis);
		t.transformCoord(vPos, vPos);
		setPosition(vPos);

	} // End if !canRotate()
	else
	{
		// Rotate a copy of the current math::transform.
		math::transform m = getTransform();
		m.rotateAxis(math::radians(degrees), axis);

		if (!mParent.expired())
		{
			math::transform invParentTransform = math::inverse(mParent.lock()->getTransform());
			m = invParentTransform * m;
		}

		setLocalTransform(m);

	} // End if canRotate()
	return *this;
}


//-----------------------------------------------------------------------------
//  Name : rotate ()
/// <summary>
/// Rotate the node by the amounts specified in world space.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::rotate(float x, float y, float z)
{
	// No - op?
	if (!x && !y && !z)
		return *this;

	// If rotation is disallowed, only process position change. Otherwise
	// perform full rotation.
	if (!canRotate())
	{
		// Scale the position, but do not allow axes to scale.
		math::transform t;
		math::vec3 position = getPosition();
		t.rotate(math::radians(x), math::radians(y), math::radians(z));
		t.transformCoord(position, position);
		setPosition(position);

	} // End if !canRotate()
	else
	{
		// Scale a copy of the cell math::transform
		// Set orientation of new math::transform
		math::transform m = getTransform();
		m.rotate(math::radians(x), math::radians(y), math::radians(z));

		if (!mParent.expired())
		{
			math::transform invParentTransform = math::inverse(mParent.lock()->getTransform());
			m = invParentTransform * m;
		}

		setLocalTransform(m);

	} // End if canRotate()
	return *this;
}

//-----------------------------------------------------------------------------
//  Name : rotate ()
/// <summary>
/// Rotate the node by the amounts specified in world space.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::rotate(float x, float y, float z, const math::vec3 & center)
{
	// No - op?
	if (!x && !y && !z)
		return *this;

	// If rotation is disallowed, only process position change. Otherwise
	// perform full rotation.
	if (!canRotate())
	{
		// Scale the position, but do not allow axes to scale.
		math::transform t;
		math::vec3 position = getPosition() - center;
		t.rotate(math::radians(x), math::radians(y), math::radians(z));
		t.transformCoord(position, position);
		setPosition(position + center);

	} // End if !canRotate()
	else
	{

		// Scale a copy of the cell math::transform
		// Set orientation of new math::transform
		math::transform m = getTransform();
		m.rotate(math::radians(x), math::radians(y), math::radians(z));

		if (!mParent.expired())
		{
			math::transform invParentTransform = math::inverse(mParent.lock()->getTransform());
			m = invParentTransform * m;
		}

		setLocalTransform(m);

	} // End if canRotate()
	return *this;
}


//-----------------------------------------------------------------------------
//  Name : scale()
/// <summary>
/// Scale the node by the specified amount in world space.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::setScale(const math::vec3& s)
{
	// If scaling is disallowed, only process position change. Otherwise
	// perform full scale.
	if (!canScale())
		return *this;

	// Scale a copy of the cell math::transform
	// Set orientation of new math::transform
	math::transform m = getTransform();
	m.setScale(s);

	if (!mParent.expired())
	{
		math::transform invParentTransform = math::inverse(mParent.lock()->getTransform());
		m = invParentTransform * m;
	}

	setLocalTransform(m);
	return *this;
}

//-----------------------------------------------------------------------------
// Name : setLocalScale() (Virtual)
/// <summary>Scale the node by the specified amount in "local" space.</summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::setLocalScale(const math::vec3 & scale)
{
	// Do nothing if scaling is disallowed.
	if (!canScale())
		return *this;
	mLocalTransform.setScale(scale);
	setLocalTransform(mLocalTransform);
	return *this;
}

//-----------------------------------------------------------------------------
//  Name : setRotation ()
/// <summary>
/// Update the node's orientation using the quaternion provided.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::setRotation(const math::quat & rotation)
{
	// Do nothing if rotation is disallowed.
	if (!canRotate())
		return *this;

	// Set orientation of new math::transform
	math::transform m = getTransform();
	m.setRotation(rotation);

	if (!mParent.expired())
	{
		math::transform invParentTransform = math::inverse(mParent.lock()->getTransform());
		m = invParentTransform * m;
	}

	setLocalTransform(m);
	return *this;
}


//-----------------------------------------------------------------------------
//  Name : setLocalRotation ()
/// <summary>
/// Update the node's orientation using the quaternion provided.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::setLocalRotation(const math::quat & rotation)
{
	// Do nothing if rotation is disallowed.
	if (!canRotate())
		return *this;

	// Set orientation of new math::transform
	mLocalTransform.setRotation(rotation);
	setLocalTransform(mLocalTransform);

	return *this;
}

//-----------------------------------------------------------------------------
// Name : resetOrientation() (Virtual)
/// <summary>
/// Simply reset the orientation of the node, maintaining the current position
/// and scale.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::resetRotation()
{
	// Do nothing if rotation is disallowed.
	if (!canRotate())
		return *this;
	setRotation(math::quat{});
	return *this;
}

//-----------------------------------------------------------------------------
// Name : resetScale() (Virtual)
/// <summary>
/// Simply reset the scale of the node, maintaining the current position
/// and orientation.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::resetScale()
{
	// Do nothing if scaling is disallowed.
	if (!canScale())
		return *this;

	setScale(math::vec3{ 1.0f, 1.0f, 1.0f });
	return *this;
}

//-----------------------------------------------------------------------------
// Name : resetOrientation() (Virtual)
/// <summary>
/// Simply reset the orientation of the node, maintaining the current position
/// and scale.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::resetLocalRotation()
{
	// Do nothing if rotation is disallowed.
	if (!canRotate())
		return *this;

	setLocalRotation(math::quat{});

	return *this;
}

//-----------------------------------------------------------------------------
// Name : resetScale() (Virtual)
/// <summary>
/// Simply reset the scale of the node, maintaining the current position
/// and orientation.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::resetLocalScale()
{
	// Do nothing if scaling is disallowed.
	if (!canScale())
		return *this;

	setLocalScale(math::vec3{ 1.0f, 1.0f, 1.0f });

	return *this;
}

//-----------------------------------------------------------------------------
// Name : resetPivot() (Virtual)
/// <summary>
/// Reset the pivot such that it sits at the origin of the object.
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::resetPivot()
{
	// Do nothing if pivot adjustment is disallowed.
	if (!canAdjustPivot())
		return *this;

	return *this;
}


//-----------------------------------------------------------------------------
// Name : canScale ( )
/// <summary>Determine if scaling of this node's math::transform is allowed.</summary>
//-----------------------------------------------------------------------------
bool TransformComponent::canScale() const
{
	// Default is to allow scaling.
	return true;
}

//-----------------------------------------------------------------------------
// Name : canRotate ( )
/// <summary>Determine if rotation of this node's math::transform is allowed.</summary>
//-----------------------------------------------------------------------------
bool TransformComponent::canRotate() const
{
	// Default is to allow rotation.
	return true;
}

//-----------------------------------------------------------------------------
// Name : canAdjustPivot ( )
/// <summary>
/// Determine if separation of this node's pivot and object space is allowed.
/// </summary>
//-----------------------------------------------------------------------------
bool TransformComponent::canAdjustPivot() const
{
	// Default is to allow pivot adjustment.
	return true;
}

//-----------------------------------------------------------------------------
//  Name : setParent ()
/// <summary>
/// Attach this node to the specified parent as a child
/// </summary>
//-----------------------------------------------------------------------------
TransformComponent& TransformComponent::setParent(ComponentHandle<TransformComponent> parent)
{
	setParent(parent, true, false);

	return *this;
}
TransformComponent& TransformComponent::setParent(ComponentHandle<TransformComponent> parent, bool worldPositionStays, bool localPositionStays)
{
	auto parentPtr = parent.lock().get();
	auto thisParentPtr = mParent.lock().get();
	// Skip if this is a no-op.
	if (thisParentPtr == parentPtr || this == parentPtr)
		return *this;
	if (parentPtr && (parentPtr->mParent.lock().get() == this))
		return *this;
	// Before we do anything, make sure that all pending math::transform 
	// operations are resolved (including those applied to our parent).
	math::transform cachedWorldTranform;
	if (worldPositionStays)
	{
		resolveTransform(true);
		cachedWorldTranform = getTransform();
	}

	if (!mParent.expired())
	{
		mParent.lock()->removeChild(makeHandle());
	}

	mParent = parent;

	if (!parent.expired())
	{
		auto shParent = parent.lock();
		// We're now attached / detached as required.	
		shParent->attachChild(makeHandle());

		mHierarchyLevel = shParent->mHierarchyLevel + 1;
	}
	else
	{
		mHierarchyLevel = 0;
	}

	if (worldPositionStays)
	{
		resolveTransform(true);
		setTransform(cachedWorldTranform);
	}
	else
	{
		if (!localPositionStays)
			setLocalTransform(math::transform::Identity);
	}


	// Success!
	return *this;
}

const ComponentHandle<TransformComponent>& TransformComponent::getParent() const
{
	return mParent;
}

void TransformComponent::attachChild(ComponentHandle<TransformComponent> child)
{
	mChildren.push_back(child);
}

void TransformComponent::removeChild(ComponentHandle<TransformComponent> child)
{
	mChildren.erase(std::remove_if(std::begin(mChildren), std::end(mChildren),
		[&child](ComponentHandle<TransformComponent> other) { return child.lock() == other.lock(); }
	), std::end(mChildren));
}

TransformComponent& TransformComponent::setTransform(const math::transform & tr)
{
	if (mWorldTransform.compare(tr, 0.0001f) == 0)
		return *this;

	math::vec3 position, scaling;
	math::quat orientation;
	tr.decompose(scaling, orientation, position);

	math::transform m = getTransform();
	m.setScale(scaling);
	m.setRotation(orientation);
	m.setPosition(position);

	if (!mParent.expired())
	{
		math::transform invParentTransform = math::inverse(mParent.lock()->getTransform());
		m = invParentTransform * m;
	}

	setLocalTransform(m);
	return *this;
}

TransformComponent& TransformComponent::setLocalTransform(const math::transform & trans)
{
	if (mLocalTransform.compare(trans, 0.0001f) == 0)
		return *this;

	static const std::string strContext = "LocalTransform";
	onModified(strContext);
	mLocalTransform = trans;
	return *this;
}

void TransformComponent::resolveTransform(bool force, float dt)
{
	bool dirty = isDirty();

	if (force || dirty)
	{
		if (!mParent.expired())
		{
			auto target = mParent.lock()->getTransform() * mLocalTransform;

			if(mSlowParenting)
			{
				float t = math::clamp(mSlowParentingSpeed * dt, 0.0f, 1.0f);
				mWorldTransform.setPosition(math::lerp(mWorldTransform.getPosition(), target.getPosition(), t));
				mWorldTransform.setScale(math::lerp(mWorldTransform.getScale(), target.getScale(), t));
				mWorldTransform.setRotation(math::slerp(mWorldTransform.getRotation(), target.getRotation(), t));
			}
			else
			{
				mWorldTransform = target;
			}	
		}
		else
		{
			mWorldTransform = mLocalTransform;
			mHierarchyLevel = 0;
		}
	}

	mDirty = false;
}

bool TransformComponent::isDirty() const
{
	bool bDirty = Component::isDirty();
	if (!bDirty && !mParent.expired())
	{
		bDirty |= mParent.lock()->isDirty();
	}

	return bDirty;
}


const std::vector<ComponentHandle<TransformComponent>>& TransformComponent::getChildren() const
{
	return mChildren;
}
