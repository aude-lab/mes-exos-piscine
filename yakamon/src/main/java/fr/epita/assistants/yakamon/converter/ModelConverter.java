package fr.epita.assistants.yakamon.converter;

import fr.epita.assistants.yakamon.data.model.*;
import fr.epita.assistants.yakamon.domain.entity.*;
import jakarta.enterprise.context.ApplicationScoped;

import java.util.List;
import java.util.stream.Collectors;


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

    public ItemEntity toItemEntity(ItemModel model) {
        if (model == null) {
            return null;
        }

        ItemEntity entity = new ItemEntity();
        entity.setId(model.getId());
        entity.setType(model.getType());
        entity.setQuantity(model.getQuantity());
        return entity;
    }


    public ItemModel toItemModel(ItemEntity entity) {
        if (entity == null) {
            return null;
        }

        ItemModel model = new ItemModel();
        model.setId(entity.getId());
        model.setType(entity.getType());
        model.setQuantity(entity.getQuantity());
        return model;
    }

    public List<ItemEntity> toItemEntityList(List<ItemModel> models) {
        if (models == null) {
            return null;
        }
        return models.stream()
                .map(this::toItemEntity)
                .collect(Collectors.toList());
    }


    public YakadexEntryEntity toYakadexEntryEntity(YakadexentryModel model) {
        if (model == null) {
            return null;
        }

        YakadexEntryEntity entity = new YakadexEntryEntity();
        entity.setId(model.getId());
        entity.setName(model.getName());
        entity.setFirstType(model.getFirstType());
        entity.setSecondType(model.getSecondType());
        entity.setDescription(model.getDescription());
        entity.setEvolutionId(model.getEvolutionId());
        entity.setEvolveThreshold(model.getEvolveThreshold());
        entity.setCaught(model.getCaught());
        return entity;
    }


    public YakadexentryModel toYakadexEntryModel(YakadexEntryEntity entity) {
        if (entity == null) {
            return null;
        }

        YakadexentryModel model = new YakadexentryModel();
        model.setId(entity.getId());
        model.setName(entity.getName());
        model.setFirstType(entity.getFirstType());
        model.setSecondType(entity.getSecondType());
        model.setDescription(entity.getDescription());
        model.setEvolutionId(entity.getEvolutionId());
        model.setEvolveThreshold(entity.getEvolveThreshold());
        model.setCaught(entity.getCaught());
        return model;
    }

    public List<YakadexEntryEntity> toYakadexEntryEntityList(List<YakadexentryModel> models) {
        if (models == null) {
            return null;
        }
        return models.stream()
                .map(this::toYakadexEntryEntity)
                .collect(Collectors.toList());
    }

    public YakamonEntity toYakamonEntity(YakamonModel model) {
        if (model == null) {
            return null;
        }

        YakamonEntity entity = new YakamonEntity();
        entity.setUuid(model.getUuid());
        entity.setNickname(model.getNickname());
        entity.setEnergyPoints(model.getEnergyPoints());
        entity.setYakadexEntryId(model.getYakadexEntryId());
        return entity;
    }

    public YakamonModel toYakamonModel(YakamonEntity entity) {
        if (entity == null) {
            return null;
        }

        YakamonModel model = new YakamonModel();
        model.setUuid(entity.getUuid());
        model.setNickname(entity.getNickname());
        model.setEnergyPoints(entity.getEnergyPoints());
        model.setYakadexEntryId(entity.getYakadexEntryId());
        return model;
    }

    public List<YakamonEntity> toYakamonEntityList(List<YakamonModel> models) {
        if (models == null) {
            return null;
        }
        return models.stream()
                .map(this::toYakamonEntity)
                .collect(Collectors.toList());
    }
}
