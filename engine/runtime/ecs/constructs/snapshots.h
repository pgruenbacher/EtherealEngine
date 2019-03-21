#pragma once

#include <core/serialization/associative_archive.h>
#include "runtime/ecs/ent.h"

namespace ecs {

// template<typename Storage>
class output_archive {
public:
    output_archive(cereal::oarchive_associative_t &storage, const Registry& reg)
        : _storage{storage}, _reg(reg)
    {
        _storage.setNextName("entities");
        _storage.startNode();
        _reg.snapshot().entities(*this);
        _storage.finishNode();
    }

    output_archive(cereal::oarchive_associative_t &storage, const Registry& reg, const std::vector<EntityType>& ents)
        : _storage{storage}, _reg(reg), _ents(ents)
    {
        _storage.setNextName("entities");
        _storage.startNode();
        _reg.snapshot().entities(*this);
        _storage.finishNode();
    }

    template<typename C>
    void set(const char* s) {
        // no need to dump if none of the components

        if (_ents.size()) {
            bool hasComp = false;
            for (auto ent : _ents) {
                if (_reg.has<C>(ent)) {
                    hasComp = true;
                    break;
                }
            }
            // dump specific ents.
            if (hasComp) {
                _storage.setNextName(s);
                _storage.startNode();
                _reg.snapshot().component<C>(*this, _ents.begin(), _ents.end());
                _storage.finishNode();
            }
        } else {
            if (_reg.view<const C>().size() > 0) {
                // dump all
                _storage.setNextName(s);
                _storage.startNode();
                _reg.snapshot().component<C>(*this);
                _storage.finishNode();
            }
        }
    }

    template<typename... Value>
    void operator()(const Value &... value) {
        // std::cout << "ASS " << std::endl;
        (_storage(value), ...);
    }

private:
    cereal::oarchive_associative_t &_storage;
    std::vector<EntityType> _ents;
    const Registry &_reg;
};
/*----------  Snapshot Archive  ----------*/

// template<typename Storage>
class input_archive {
public:
    input_archive(cereal::iarchive_associative_t &storage, Registry& reg)
        : _storage{storage}, _loader(reg.loader())
    {
        _storage.setNextName("entities");
        _storage.startNode();
        _loader.entities(*this);
        _storage.finishNode();

    }

    template<typename C>
    void get(const char* s) {
        try {
            _storage.setNextName(s);
            _storage.startNode();
            _loader.component<C>(*this);
            _storage.finishNode();
        } catch (const cereal::Exception& e) {
            return;
        }
    }

    template<typename... Value>
    void operator()(Value &... value) {
        (_storage(value), ...);
    }

private:
    cereal::iarchive_associative_t& _storage;
    entt::snapshot_loader _loader;
};

/*----------  Continuous Archive  ----------*/


// template<typename Storage>
class continuous_archive {
public:
    continuous_archive(cereal::iarchive_associative_t &storage, Registry& reg)
        : _storage{storage}, _loader(reg)
    {
        _storage.setNextName("entities");
        _storage.startNode();
        _loader.entities(*this);
        _storage.finishNode();
    }

    template<typename C, typename... Type, typename... Member>
    void get(const char* s, Member Type:: *... member) {
        try {
            _storage.setNextName(s);
            _storage.startNode();
            _loader.component<C>(*this, member...);
            _storage.finishNode();
        } catch (const cereal::Exception& e) {
            return;
        }
    }

    template<typename... Value>
    void operator()(Value &... value) {
        (_storage(value), ...);
    }

private:
    cereal::iarchive_associative_t& _storage;
    entt::continuous_loader _loader;
};


EntityType clone(Registry& reg, EntityType ent);
void deserialize(std::istream& stream, Registry& reg, std::vector<EntityType>& output);
void serialize(std::ostream& stream, const Registry& reg, const std::vector<EntityType>& ents);

}  // namespace ecs
