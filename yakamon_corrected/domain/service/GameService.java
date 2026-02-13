package fr.epita.assistants.yakamon.domain.service;

import fr.epita.assistants.yakamon.converter.ModelConverter;
import fr.epita.assistants.yakamon.data.model.GameModel;
import fr.epita.assistants.yakamon.data.repository.GameRepository;
import fr.epita.assistants.yakamon.domain.entity.GameEntity;
import fr.epita.assistants.yakamon.utils.tile.ItemType;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

@ApplicationScoped
public class GameService {

    @Inject
    GameRepository gameRepository;

    @Inject
    PlayerService playerService;

    @Inject
    ItemService itemService;

    @Inject
    YakadexEntryService yakadexEntryService;

    @Inject
    ModelConverter modelConverter;  // ✅ Utilise ModelConverter pour les conversions Model ↔ Entity

    /**
     * ✅ Récupère le jeu et convertit Model → Entity
     */
    public GameEntity getGame() {
        GameModel model = gameRepository.findGame();
        return modelConverter.toGameEntity(model);
    }

    /**
     * ✅ Crée un nouveau jeu
     * - Travaille avec GameEntity (Business Layer)
     * - Utilise le converter pour transformer Entity → Model avant de sauvegarder
     * - Reconvertit Model → Entity pour le retour
     */
    @Transactional
    public GameEntity createGame(String mapFilePath) throws IOException {
        String parsedMap = parseMapFile(mapFilePath);

        // Travaille avec Entity
        GameEntity entity = new GameEntity();
        entity.setMap(parsedMap);

        // Convertit Entity → Model pour le Repository
        GameModel model = modelConverter.toGameModel(entity);
        GameModel savedModel = gameRepository.saveGame(model);

        // Reconvertit Model → Entity pour le retour
        return modelConverter.toGameEntity(savedModel);
    }

    /**
     * Parse le fichier de map au format RLE simplifié
     */
    private String parseMapFile(String mapFilePath) throws IOException {
        Path path = Path.of(mapFilePath);
        List<String> lines = Files.readAllLines(path);
        return String.join(";", lines);
    }

    /**
     * Vérifie si un jeu existe
     */
    public boolean gameExists() {
        return getGame() != null;
    }

    /**
     * ✅ Démarre un nouveau jeu
     * - Réinitialise toutes les tables
     * - Crée un nouveau joueur
     * - Ajoute 5 yakaballs à l'inventaire
     * - Crée le jeu avec la map
     */
    @Transactional
    public GameEntity startNewGame(String playerName, String mapPath) {
        // Réinitialisation
        playerService.deleteAll();
        itemService.deleteAll();
        deleteAll();
        yakadexEntryService.resetAllCaught();

        // Création du joueur et ajout des yakaballs
        playerService.createPlayer(playerName);
        itemService.addItem(ItemType.YAKABALL, 5);

        // Création du jeu
        try {
            return createGame(mapPath);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Supprime tous les jeux
     */
    @Transactional
    public void deleteAll() {
        gameRepository.deleteAll();
    }
}
