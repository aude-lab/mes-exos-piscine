package fr.epita.assistants.yakamon.domain.service;

import fr.epita.assistants.yakamon.converter.ModelConverter;
import fr.epita.assistants.yakamon.data.model.PlayerModel;
import fr.epita.assistants.yakamon.data.repository.PlayerRepository;
import fr.epita.assistants.yakamon.domain.entity.PlayerEntity;
import fr.epita.assistants.yakamon.utils.Direction;
import fr.epita.assistants.yakamon.utils.Point;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;

import java.time.LocalDateTime;

@ApplicationScoped
public class PlayerService {

    @Inject
    PlayerRepository playerRepository;
    @Inject
    ModelConverter modelConverter;

    public PlayerEntity getPlayer() {
        PlayerModel model = playerRepository.findPlayer();
        return modelConverter.toPlayerEntity(model);
    }


    @Transactional
    public PlayerEntity createPlayer(String name) {
        PlayerEntity entity = new PlayerEntity();
        entity.setName(name);
        entity.setPosX(0);
        entity.setPosY(0);
        entity.setLastMove(null);
        entity.setLastCatch(null);
        entity.setLastCollect(null);
        entity.setLastFeed(null);

        PlayerModel model = modelConverter.toPlayerModel(entity);
        PlayerModel savedModel = playerRepository.savePlayer(model);

        return modelConverter.toPlayerEntity(savedModel);
    }


    @Transactional
    public void updatePosition(Integer newX, Integer newY) {
        PlayerEntity entity = getPlayer();
        if (entity != null) {
            entity.setPosX(newX);
            entity.setPosY(newY);
            entity.setLastMove(LocalDateTime.now());

            PlayerModel model = modelConverter.toPlayerModel(entity);
            playerRepository.savePlayer(model);
        }
    }


    @Transactional
    public void movePlayer(Direction direction) {
        PlayerEntity entity = getPlayer();
        if (entity != null) {
            Point movePoint = direction.getPoint();
            Integer newX = entity.getPosX() + movePoint.getPosX();
            Integer newY = entity.getPosY() + movePoint.getPosY();
            updatePosition(newX, newY);
        }
    }


    @Transactional
    public void updateLastCatch() {
        PlayerEntity entity = getPlayer();
        if (entity != null) {
            entity.setLastCatch(LocalDateTime.now());

            PlayerModel model = modelConverter.toPlayerModel(entity);
            playerRepository.savePlayer(model);
        }
    }

    @Transactional
    public void updateLastCollect() {
        PlayerEntity entity = getPlayer();
        if (entity != null) {
            entity.setLastCollect(LocalDateTime.now());

            PlayerModel model = modelConverter.toPlayerModel(entity);
            playerRepository.savePlayer(model);
        }
    }

    @Transactional
    public void updateLastFeed() {
        PlayerEntity entity = getPlayer();
        if (entity != null) {
            entity.setLastFeed(LocalDateTime.now());

            PlayerModel model = modelConverter.toPlayerModel(entity);
            playerRepository.savePlayer(model);
        }
    }

    public boolean playerExists() {
        return getPlayer() != null;
    }


    @Transactional
    public void deletedAll() {
        playerRepository.deletedAll();
    }
}
