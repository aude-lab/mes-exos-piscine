package fr.epita.assistants.yakamon.converter;

import fr.epita.assistants.yakamon.domain.entity.PlayerEntity;
import fr.epita.assistants.yakamon.presentation.api.response.PlayerResponse;
import jakarta.enterprise.context.ApplicationScoped;

@ApplicationScoped
public class PlayerConverter {

    public PlayerResponse toResponse(PlayerEntity entity) {
        PlayerResponse response = new PlayerResponse();

        response.setId(entity.getUuid());
        response.setName(entity.getName());
        response.setPosX(entity.getPosX());
        response.setPosY(entity.getPosY());
        response.setLastMove(entity.getLastMove());
        response.setLastCollect(entity.getLastCollect());
        response.setLastCatch(entity.getLastCatch());
        response.setLastFeed(entity.getLastFeed());

        return response;
    }
}
