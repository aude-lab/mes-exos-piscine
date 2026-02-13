package fr.epita.assistants.yakamon.domain.service;

import fr.epita.assistants.yakamon.data.repository.GameRepository;
import fr.epita.assistants.yakamon.domain.entity.GameEntity;
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

    public GameEntity getGame() {
        return gameRepository.findGame();
    }

    @Transactional
    public GameEntity createGame(String mapFilePath) throws IOException {
        String parsedMap = parseMapFile(mapFilePath);
        
        GameEntity game = new GameEntity();
        game.setMap(parsedMap);
        
        return gameRepository.saveGame(game);
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
    playerService.deleteAll();
    itemService.deleteAll();
    deleteAll();
    yakadexEntryService.resetAllCaught();
    playerService.createPlayer(playerName);
    itemService.addItem(ItemType.YAKABALL, 5);
    return createGame(mapPath);
}

    @Transactional
    public void deleteAll() {
        gameRepository.deleteAll();
    }
}
