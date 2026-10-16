#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <unordered_map>
#include <unordered_set>

using namespace geode::prelude;

namespace {
    struct ScaleState {
        float scaleX = 1.f;
        float scaleY = 1.f;
    };

    std::unordered_map<GameObject*, ScaleState> g_spikeBaseScales;
    std::unordered_map<PlayerObject*, ScaleState> g_playerBaseScales;

    bool isSizeMayhemSpike(int objectID) {
        switch (objectID) {
            case 8:     // Standard spike
            case 39:    // Small spike
            case 40:    // Alternate spike
            case 103:   // Upside-down spike
            case 392:   // Spike variant
            case 1333:  // Spike variant
            case 1334:  // Spike variant
                return true;
            default:
                return false;
        }
    }

    void applyPlayerScale(PlayerObject* player, bool enabled, float multiplier) {
        if (!player) {
            return;
        }

        auto state = g_playerBaseScales.find(player);
        if (enabled) {
            if (state == g_playerBaseScales.end()) {
                state = g_playerBaseScales.emplace(
                    player,
                    ScaleState{player->getScaleX(), player->getScaleY()}
                ).first;
            }
            player->setScaleX(state->second.scaleX * multiplier);
            player->setScaleY(state->second.scaleY * multiplier);
        } else if (state != g_playerBaseScales.end()) {
            player->setScaleX(state->second.scaleX);
            player->setScaleY(state->second.scaleY);
            g_playerBaseScales.erase(state);
        }
    }
}

class $modify(SizeMayhemPlayLayer, PlayLayer) {
    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        auto mod = Mod::get();
        const bool enabled = mod->getSettingValue<bool>("enabled");
        const float playerMultiplier = enabled
            ? static_cast<float>(mod->getSettingValue<double>("player-scale"))
            : 1.f;
        const float spikeMultiplier = enabled
            ? static_cast<float>(mod->getSettingValue<double>("spike-scale"))
            : 1.f;

        std::unordered_set<PlayerObject*> activePlayers;
        if (m_player1) {
            activePlayers.insert(m_player1);
            applyPlayerScale(m_player1, enabled, playerMultiplier);
        }
        if (m_player2) {
            activePlayers.insert(m_player2);
            applyPlayerScale(m_player2, enabled, playerMultiplier);
        }

        for (auto it = g_playerBaseScales.begin(); it != g_playerBaseScales.end();) {
            if (!activePlayers.contains(it->first)) {
                it = g_playerBaseScales.erase(it);
            } else {
                ++it;
            }
        }

        std::unordered_set<GameObject*> activeSpikes;
        if (m_objects) {
            for (unsigned int i = 0; i < m_objects->count(); ++i) {
                auto object = static_cast<GameObject*>(m_objects->objectAtIndex(i));
                if (!object || !isSizeMayhemSpike(object->m_objectID)) {
                continue;
            }

            activeSpikes.insert(object);
            auto state = g_spikeBaseScales.find(object);

            if (enabled) {
                if (state == g_spikeBaseScales.end()) {
                    state = g_spikeBaseScales.emplace(
                        object,
                        ScaleState{object->getScaleX(), object->getScaleY()}
                    ).first;
                }

                object->setScaleX(state->second.scaleX * spikeMultiplier);
                object->setScaleY(state->second.scaleY * spikeMultiplier);
                } else if (state != g_spikeBaseScales.end()) {
                    object->setScaleX(state->second.scaleX);
                    object->setScaleY(state->second.scaleY);
                    g_spikeBaseScales.erase(state);
                }
            }
        }

        for (auto it = g_spikeBaseScales.begin(); it != g_spikeBaseScales.end();) {
            if (!activeSpikes.contains(it->first)) {
                it = g_spikeBaseScales.erase(it);
            } else {
                ++it;
            }
        }
    }
};
