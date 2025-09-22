
#include "stage/scene/component/sound.h"

#include "stage/scene/node.h"

namespace soil::stage::scene::component {
    Sound::Sound(sound::Source* source, const bool deleteAfterPlayed) :
        Component(Type::Sound), source_(source), deleteAfterPlayed_(deleteAfterPlayed) {
        if (deleteAfterPlayed) {
            source_->AddListener(this);
        }
    }

    Sound::~Sound() {
        if (deleteAfterPlayed_) {
            source_->RemoveListener(this);
        }
        delete source_;
    }

    void Sound::Handle(const sound::event::Event& event) {
        if (event.Cause() != sound::event::Cause::Source) {
            return;
        }
        auto& sourceEvent = dynamic_cast<const sound::event::SourceEvent&>(event);
        if (sourceEvent.Trigger() != sound::event::SourceEvent::TriggerType::PlayStateChanged) {
            return;
        }
        if (deleteAfterPlayed_ && !sourceEvent.Source()->IsPlaying()) {
            delete this; // suicide
        }
    }

    void Sound::Play() const {
        if (source_ == nullptr) {
            return;
        }
        source_->Play();
    }

    void Sound::Stop() const {
        if (source_ == nullptr) {
            return;
        }
        source_->Stop();
    }

    sound::Source& Sound::Source() const {
        return *source_;
    }

    void Sound::UpdateTransform(const glm::mat4& matrix) {
        Component::UpdateTransform(matrix);
        if (source_ == nullptr) {
            return;
        }
        source_->SetPosition(matrix[3]);
    }
} // namespace soil::stage::scene::component
