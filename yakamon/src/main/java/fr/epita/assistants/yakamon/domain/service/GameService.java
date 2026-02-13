package fr.epita.assistants.yakamon.domain.service;

import fr.epita.assistants.yakamon.data.model.GameModel;
import fr.epita.assistants.yakamon.data.repository.GameRepository;
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
    public GameModel getGame() {
        return gameRepository.findGame();
    }

    @Transactional
    public GameModel createGame(String mapFilePath) throws IOException {
        String parsedMap = parseMapFile(mapFilePath);

        GameModel game = new GameModel();
        game.setMap(parsedMap);
        gameRepository.saveGame(game);

        return game;
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
    public void deletedAll() {
        gameRepository.deletedAll();
    }
}
