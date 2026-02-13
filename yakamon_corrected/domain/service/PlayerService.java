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
    ModelConverter modelConverter;  // ✅ Injecte le converter

    /**
     * ✅ Récupère le joueur
     * - Repository retourne un PlayerModel
     * - Converter transforme Model → Entity
     */
    public PlayerEntity getPlayer() {
        PlayerModel model = playerRepository.findPlayer();
        return modelConverter.toPlayerEntity(model);
    }

    /**
     * ✅ Crée un nouveau joueur
     * - Travaille avec PlayerEntity
     * - Convertit Entity → Model avant de sauvegarder
     * - Reconvertit Model → Entity pour le retour
     */
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

        // Convertit Entity → Model
        PlayerModel model = modelConverter.toPlayerModel(entity);
        PlayerModel savedModel = playerRepository.savePlayer(model);

        // Reconvertit Model → Entity
        return modelConverter.toPlayerEntity(savedModel);
    }

    /**
     * ✅ Met à jour la position du joueur
     */
    @Transactional
    public void updatePosition(Integer newX, Integer newY) {
        PlayerEntity entity = getPlayer();
        if (entity != null) {
            entity.setPosX(newX);
            entity.setPosY(newY);
            entity.setLastMove(LocalDateTime.now());

            // Convertit et sauvegarde
            PlayerModel model = modelConverter.toPlayerModel(entity);
            playerRepository.savePlayer(model);
        }
    }

    /**
     * ✅ Déplace le joueur dans une direction
     */
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

    /**
     * ✅ Met à jour la dernière capture
     */
    @Transactional
    public void updateLastCatch() {
        PlayerEntity entity = getPlayer();
        if (entity != null) {
            entity.setLastCatch(LocalDateTime.now());

            PlayerModel model = modelConverter.toPlayerModel(entity);
            playerRepository.savePlayer(model);
        }
    }

    /**
     * ✅ Met à jour la dernière collecte
     */
    @Transactional
    public void updateLastCollect() {
        PlayerEntity entity = getPlayer();
        if (entity != null) {
            entity.setLastCollect(LocalDateTime.now());

            PlayerModel model = modelConverter.toPlayerModel(entity);
            playerRepository.savePlayer(model);
        }
    }

    /**
     * ✅ Met à jour le dernier feed
     */
    @Transactional
    public void updateLastFeed() {
        PlayerEntity entity = getPlayer();
        if (entity != null) {
            entity.setLastFeed(LocalDateTime.now());

            PlayerModel model = modelConverter.toPlayerModel(entity);
            playerRepository.savePlayer(model);
        }
    }

    /**
     * Vérifie si un joueur existe
     */
    public boolean playerExists() {
        return getPlayer() != null;
    }

    /**
     * Supprime tous les joueurs
     */
    @Transactional
    public void deleteAll() {
        playerRepository.deleteAll();
    }
}
