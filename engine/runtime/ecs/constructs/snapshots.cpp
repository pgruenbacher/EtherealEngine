#include "./snapshots.h"
#include <core/serialization/associative_archive.h>
#include <core/serialization/binary_archive.h>

#include "runtime/ecs/components/audio_listener_component.h"
#include "runtime/ecs/components/audio_source_component.h"
#include "runtime/ecs/components/camera_component.h"
#include "runtime/ecs/components/light_component.h"
#include "runtime/ecs/components/model_component.h"
#include "runtime/ecs/components/reflection_probe_component.h"
#include "runtime/ecs/components/relation.h"
#include "runtime/ecs/components/transform_component.h"

namespace ecs {

template<typename Archive>
void serialize_t(std::ostream& stream, const Registry& reg, const std::vector<EntityType>& ents)
{
    Archive ar(stream);
    output_archive snap(ar, reg, ents);

    snap.set<audio_listener_component>("audio_listenero_listener");
    snap.set<audio_source_component>("audio_source_component");
    snap.set<camera_component>("camera_component");
    snap.set<light_component>("light_component");
    snap.set<model_component>("model_component");
    snap.set<reflection_probe_component>("reflection_probe_component");
    snap.set<Relation>("relation");
    snap.set<transform_component>("transform_component");
    snap.set<Name>("name");
}

template<typename Archive>
void deserialize_t(std::istream& stream, Registry& reg)
{
    Archive ar(stream);
    continuous_archive loader(ar, reg);
    loader.get<audio_listener_component>("audio_listener");
    loader.get<audio_source_component>("audio_source_component");
    loader.get<camera_component>("camera_component");
    loader.get<light_component>("light_component");
    loader.get<model_component>("model_component");
    loader.get<reflection_probe_component>("reflection_probe_component");
    loader.get<Relation>("relation", &Relation::parent);
    loader.get<transform_component>("transform_component");
    loader.get<Name>("name");

}

// struct catcher {
//     void set_last_ent(Registry& reg, EntityType ent) {
//         _last_ent = ent;
//     }
//     EntityType _last_ent;
// };


struct assign_defaults {
    void on_new_ent(Registry& reg, EntityType ent) {
        // _last_ent = ent;
        reg.assign<MarkDelete>(ent);
        ents.push_back(ent);
    }
    std::vector<EntityType>& ents;
    assign_defaults(std::vector<EntityType>& ents) : ents(ents) {}
    // EntityType _last_ent;
};


EntityType clone(Registry& reg, EntityType ent)
{
    std::stringstream stream;
    {
        serialize_t<cereal::oarchive_associative_t>(stream, reg, {ent});
    }
    // EntityType cloned;
    // catcher c;
    // we can use Name as default componnet
    // reg.construction<Name>().connect<&catcher::set_last_ent>(&c);
    std::vector<EntityType> result;
    deserialize(stream, reg, result);
    assert(result.size() == 1);
    if (result.size() == 0) return entt::null;
    // reg.construction<Name>().disconnect<&catcher::set_last_ent>(&c);
    return result[0];

}

void deserialize(std::istream& stream, Registry& reg, std::vector<EntityType>& output) {
    assign_defaults assigner(output);

    reg.construction<Name>().connect<&assign_defaults::on_new_ent>(&assigner);
    deserialize_t<cereal::iarchive_associative_t>(stream, reg);
    reg.construction<Name>().disconnect<&assign_defaults::on_new_ent>(&assigner);
}

void serialize(std::ostream& stream, const Registry& reg, const std::vector<EntityType>& ents) {
    serialize_t<cereal::oarchive_associative_t>(stream, reg, ents);
}

} // namespace ecs
