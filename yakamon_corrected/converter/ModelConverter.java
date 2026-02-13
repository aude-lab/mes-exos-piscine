package fr.epita.assistants.yakamon.converter;

import fr.epita.assistants.yakamon.data.model.GameModel;
import fr.epita.assistants.yakamon.data.model.ItemModel;
import fr.epita.assistants.yakamon.data.model.PlayerModel;
import fr.epita.assistants.yakamon.data.model.YakadexentryModel;
import fr.epita.assistants.yakamon.domain.entity.GameEntity;
import fr.epita.assistants.yakamon.domain.entity.ItemEntity;
import fr.epita.assistants.yakamon.domain.entity.PlayerEntity;
import fr.epita.assistants.yakamon.domain.entity.YakadexEntryEntity;
import jakarta.enterprise.context.ApplicationScoped;

import java.util.List;
import java.util.stream.Collectors;

/**
 * ✅ Converter pour transformer les Models (Data Layer) en Entities (Business Layer) et vice-versa
 * Ce converter est utilisé par les Services pour communiquer avec les Repositories
 */
@ApplicationScoped
public class ModelConverter {

    // ========================================
    // PLAYER CONVERSIONS
    // ========================================

    /**
     * Convertit PlayerModel → PlayerEntity
     */
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

    /**
     * Convertit PlayerEntity → PlayerModel
     */
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

    // ========================================
    // GAME CONVERSIONS
    // ========================================

    /**
     * Convertit GameModel → GameEntity
     */
    public GameEntity toGameEntity(GameModel model) {
        if (model == null) {
            return null;
        }

        GameEntity entity = new GameEntity();
        entity.setId(model.getId());
        entity.setMap(model.getMap());
        return entity;
    }

    /**
     * Convertit GameEntity → GameModel
     */
    public GameModel toGameModel(GameEntity entity) {
        if (entity == null) {
            return null;
        }

        GameModel model = new GameModel();
        model.setId(entity.getId());
        model.setMap(entity.getMap());
        return model;
    }

    // ========================================
    // ITEM CONVERSIONS
    // ========================================

    /**
     * Convertit ItemModel → ItemEntity
     */
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

    /**
     * Convertit ItemEntity → ItemModel
     */
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

    /**
     * Convertit une liste de ItemModel → Liste de ItemEntity
     */
    public List<ItemEntity> toItemEntityList(List<ItemModel> models) {
        if (models == null) {
            return null;
        }
        return models.stream()
                .map(this::toItemEntity)
                .collect(Collectors.toList());
    }

    // ========================================
    // YAKADEX ENTRY CONVERSIONS
    // ========================================

    /**
     * Convertit YakadexentryModel → YakadexEntryEntity
     */
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

    /**
     * Convertit YakadexEntryEntity → YakadexentryModel
     */
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

    /**
     * Convertit une liste de YakadexentryModel → Liste de YakadexEntryEntity
     */
    public List<YakadexEntryEntity> toYakadexEntryEntityList(List<YakadexentryModel> models) {
        if (models == null) {
            return null;
        }
        return models.stream()
                .map(this::toYakadexEntryEntity)
                .collect(Collectors.toList());
    }
}
