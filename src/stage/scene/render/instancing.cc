#include "stage/scene/render/instancing.h"

#include <ranges>
#include <stdexcept>

#include "stage/scene/component/instance_data.h"

namespace soil::stage::scene::render {

    Instancing::~Instancing() {
        for (const auto *va : vertexArrays_) {
            delete va;
        }
        for (const auto &states : renderBatchPerKey_ | std::views::values) {
            delete states.Opaque;
            delete states.NonOpaque;
        }
    }

    void Instancing::ComponentAdded(component::VisualComponent *component) {
#ifdef DEBUG
        if (component->GetRenderType() != Type::Instancing) {
            throw std::runtime_error("component is not instanced type");
        }
#endif
        auto *instance = dynamic_cast<component::InstanceData *>(component);
        if (!renderBatchPerKey_.contains(instance->GetBatchKey())) {
            renderBatchPerKey_[instance->GetBatchKey()] = StateBatches();
        }
        auto &states = renderBatchPerKey_[instance->GetBatchKey()];
        if (instance->IsOpaque()) {
            if (states.Opaque == nullptr) {
                states.Opaque = createBatch(instance->GetBatchKey());
                addedOpaqueBatches_.push_back(states.Opaque);
            }
            states.Opaque->AddNewInstance(instance);
        } else {
            if (states.NonOpaque == nullptr) {
                states.NonOpaque = createBatch(instance->GetBatchKey());
                addedNonOpaqueBatches_.push_back(states.NonOpaque);
            }
            states.NonOpaque->AddNewInstance(instance);
        }
    }

    void Instancing::ComponentChanged(component::VisualComponent *component) {
#ifdef DEBUG
        if (component->GetRenderType() != Type::Instancing) {
            throw std::runtime_error("component is not instanced type");
        }
#endif
        if (!component->IsVisible()) {
            ComponentRemoved(component);
            return;
        }
        auto *instance = dynamic_cast<component::InstanceData *>(component);
        if (!renderBatchPerKey_.contains(instance->GetBatchKey())) {
            throw std::runtime_error("Batch with key '" + instance->GetBatchKey() + "' unknown");
        }
        auto &states = renderBatchPerKey_[instance->GetBatchKey()];
        if (instance->IsOpaque()) {
            if (states.NonOpaque != nullptr) {
                if (states.NonOpaque->RemoveInstance(instance) == true) {
                    instance->SetIndex(-1);
                    ComponentAdded(component);
                    return;
                }
            }
            if (states.Opaque == nullptr) {
                ComponentAdded(component);
                return;
            }
            states.Opaque->AddChangedInstance(instance);
        } else {
            if (states.Opaque != nullptr) {
                if (states.Opaque->RemoveInstance(instance) == true) {
                    instance->SetIndex(-1);
                    ComponentAdded(component);
                    return;
                }
            }
            if (states.NonOpaque == nullptr) {
                ComponentAdded(component);
                return;
            }
            states.NonOpaque->AddChangedInstance(instance);
        }
        // TODO remove batch if empty
    }

    void Instancing::ComponentRemoved(component::VisualComponent *component) {
#ifdef DEBUG
        if (component->GetRenderType() != Type::Instancing) {
            throw std::runtime_error("component is not instanced type");
        }
#endif

        auto *instance = dynamic_cast<component::InstanceData *>(component);
        if (!renderBatchPerKey_.contains(instance->GetBatchKey())) {
            throw std::runtime_error("Batch with key '" + instance->GetBatchKey() + "' unknown");
        }
        const auto &states = renderBatchPerKey_[instance->GetBatchKey()];
        if (instance->IsOpaque()) {
            states.Opaque->RemoveInstance(instance);
        } else {
            states.NonOpaque->RemoveInstance(instance);
        }
        // TODO remove batch if empty
    }

    void Instancing::Update(video::render::Container *container) {
        for (const auto &states : renderBatchPerKey_ | std::views::values) {
            if (states.Opaque != nullptr) {
                states.Opaque->Update({});
            }
            if (states.NonOpaque != nullptr) {
                states.NonOpaque->Update({});
            }
        }
        for (auto *batch : addedOpaqueBatches_) {
            container->Add(batch, video::render::Container::OPAQUE);
        }
        addedOpaqueBatches_.clear();

        for (auto *batch : addedNonOpaqueBatches_) {
            container->Add(batch, video::render::Container::NON_OPAQUE);
        }
        addedNonOpaqueBatches_.clear();

        for (auto *batch : removedOpaqueBatches_) {
            container->Add(batch, video::render::Container::OPAQUE);
        }
        removedOpaqueBatches_.clear();

        for (auto *batch : removedNonOpaqueBatches_) {
            container->Add(batch, video::render::Container::NON_OPAQUE);
        }
        removedNonOpaqueBatches_.clear();
    }

    void Instancing::AddRenderBatch(const std::string &key, const BatchCreationArgs &args) {
        if (batchCreationArgs_.contains(key)) {
            throw std::runtime_error("Batch with key '" + key + "' already exists");
        }
        batchCreationArgs_[key] = args;
    }

    video::render::Batch *Instancing::createBatch(const std::string &key) {
        const auto &args = batchCreationArgs_[key];
        auto *va = new video::render::VertexArray(*args.Mesh);
        vertexArrays_.push_back(va);

        GLsizei instanceSize = 0;
        for (const auto [Elements, Type] : args.VertexAttribDescriptors) {
            instanceSize += video::vertex::AttributePointer::GetSizeOfDataType(Type) * Elements;
        }

        const auto batchDescriptor =
            video::render::BatchDescriptor{.Vao = va->GetVao(),
                                           .Shader = args.Shader,
                                           .InstanceSize = instanceSize,
                                           .PreserveInstances = args.PreserveInstances,
                                           .DrawMode = args.Mesh->GetDrawMode(),
                                           .VertexAttribDescriptors = args.VertexAttribDescriptors};
        return new video::render::Batch(batchDescriptor);
    }

} // namespace soil::stage::scene::render
