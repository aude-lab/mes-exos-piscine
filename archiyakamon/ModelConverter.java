package fr.epita.assistants.yakamon.converter;

import fr.epita.assistants.yakamon.data.model.GameModel;
import fr.epita.assistants.yakamon.data.model.PlayerModel;
import fr.epita.assistants.yakamon.domain.entity.GameEntity;
import fr.epita.assistants.yakamon.domain.entity.PlayerEntity;
import jakarta.enterprise.context.ApplicationScoped;

@ApplicationScoped
public class ModelConverter {

    public PlayerEntity toPlayerEntity(PlayerModel model) {
        if (model == null) {
            return null;
        }
        
        PlayerEntity entity = new PlayerEntity();
        entity.setUuid(model.getUuid());
        entity.setName(model.getName());
        entity.setPosX(model.getPosX());
        entity.setPosY(model.getPosY());
        entity.setLastMove(model.getLastMove());
        entity.setLastCatch(model.getLastCatch());
        entity.setLastCollect(model.getLastCollect());
        entity.setLastFeed(model.getLastFeed());
        return entity;
    }

    public PlayerModel toPlayerModel(PlayerEntity entity) {
        if (entity == null) {
            return null;
        }
        
        PlayerModel model = new PlayerModel();
        model.setUuid(entity.getUuid());
        model.setName(entity.getName());
        model.setPosX(entity.getPosX());
        model.setPosY(entity.getPosY());
        model.setLastMove(entity.getLastMove());
        model.setLastCatch(entity.getLastCatch());
        model.setLastCollect(entity.getLastCollect());
        model.setLastFeed(entity.getLastFeed());
        return model;
    }

    public GameEntity toGameEntity(GameModel model) {
        if (model == null) {
            return null;
        }
        
        GameEntity entity = new GameEntity();
        entity.setId(model.getId());
        entity.setMap(model.getMap());
        return entity;
    }

    public GameModel toGameModel(GameEntity entity) {
        if (entity == null) {
            return null;
        }
        
        GameModel model = new GameModel();
        model.setId(entity.getId());
        model.setMap(entity.getMap());
        return model;
    }
}
