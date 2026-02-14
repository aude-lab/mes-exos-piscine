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
    ModelConverter modelConverter;

    public GameEntity getGame() {
        GameModel model = gameRepository.findGame();
        return modelConverter.toGameEntity(model);
    }


    @Transactional
    public GameEntity createGame(String mapFilePath) throws IOException {
        String parsedMap = parseMapFile(mapFilePath);

        GameEntity entity = new GameEntity();
        entity.setMap(parsedMap);

        GameModel model = modelConverter.toGameModel(entity);
        GameModel savedModel = gameRepository.saveGame(model);

        return modelConverter.toGameEntity(savedModel);
    }


    private String parseMapFile(String mapFilePath) throws IOException {
        Path path = Path.of(mapFilePath);
        List<String> lines = Files.readAllLines(path);
        return String.join(";", lines);
    }


    public boolean gameExists() {
        return getGame() != null;
    }


    @Transactional
    public GameEntity startNewGame(String playerName, String mapPath) {
        playerService.deletedAll();
        itemService.deleteAll();
        deleteAll();
        yakadexEntryService.resetAllCaught();

        playerService.createPlayer(playerName);
        itemService.addItem(ItemType.YAKABALL, 5);

       try {
            return createGame(mapPath);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }


    @Transactional
    public void deleteAll() {
        gameRepository.deleteAll();
    }
}




