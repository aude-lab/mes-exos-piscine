package fr.epita.assistants.yakamon.domain.service;

import fr.epita.assistants.yakamon.data.model.PlayerModel;
import fr.epita.assistants.yakamon.data.repository.PlayerRepository;
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

    public PlayerModel getPlayer() {
        return playerRepository.findPlayer();
    }

    @Transactional
    public PlayerModel createPlayer(String name) {
        PlayerModel player = new PlayerModel();
        player.setName(name);
        player.setPosX(0);
        player.setPosY(0);
        player.setLastMove(null);
        player.setLastCatch(null);
        player.setLastCollect(null);
        player.setLastFeed(null);

        playerRepository.savePlayer(player);
        return player;
    }
    @Transactional
    public void updatePosition(Integer newX, Integer newY) {
        PlayerModel player = getPlayer();
        if (player != null) {
            player.setPosX(newX);
            player.setPosY(newY);
            player.setLastMove(LocalDateTime.now());
            playerRepository.savePlayer(player);
        }
    }

    @Transactional
    public void movePlayer(Direction direction) {
        PlayerModel player = getPlayer();
        if (player != null) {
            Point movePoint = direction.getPoint();
            Integer newX = player.getPosX() + movePoint.getPosX();
            Integer newY = player.getPosY() + movePoint.getPosY();
            updatePosition(newX, newY);
        }
    }

    @Transactional
    public void updateLastCatch() {
        PlayerModel player = getPlayer();
        if (player != null) {
            player.setLastCatch(LocalDateTime.now());
            playerRepository.savePlayer(player);
        }
    }

    @Transactional
    public void updateLastCollect() {
        PlayerModel player = getPlayer();
        if (player != null) {
            player.setLastCollect(LocalDateTime.now());
            playerRepository.savePlayer(player);
        }
    }

    @Transactional
    public void updateLastFeed() {
        PlayerModel player = getPlayer();
        if (player != null) {
            player.setLastFeed(LocalDateTime.now());
            playerRepository.savePlayer(player);
        }
    }

    public boolean playerExists() {
        return getPlayer() != null;
    }
    @Transactional
    public void deletedAll() {
        playerRepository.deleteAll();
    }
}
