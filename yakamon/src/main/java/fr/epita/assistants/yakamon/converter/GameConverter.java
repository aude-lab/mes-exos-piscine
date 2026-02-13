package fr.epita.assistants.yakamon.converter;
import fr.epita.assistants.yakamon.data.model.GameModel;
import fr.epita.assistants.yakamon.presentation.api.response.StartResponse;
import fr.epita.assistants.yakamon.utils.tile.*;
import jakarta.enterprise.context.ApplicationScoped;

import java.util.ArrayList;
import java.util.List;
@ApplicationScoped

public class GameConverter {
    public StartResponse toStartResponse(GameModel game) {
        TileType[][] tiles = parserleMap(game.getMap());

        StartResponse response = new StartResponse();
        response.setTiles(tiles);
        return response;
    }

    private TileType[][] parserleMap(String rleMap) {
        String[] lines = rleMap.split(";");
        List<TileType[]> tileRows = new ArrayList<>();
        for (String line : lines) {
            TileType[] row = parserleLine(line);
            tileRows.add(row);
        }
        return tileRows.toArray(new TileType[0][]);
    }

    private TileType[] parserleLine(String line) {
        List<TileType> tiles = new ArrayList<>();

        int i = 0;
        while (i < line.length()) {
            int count = Character.getNumericValue(line.charAt(i));
            i++;
            char terrainChar = line.charAt(i);
            TerrainType terrain = TerrainType.getTerrain(terrainChar);
            i++;
            char collectibleChar = line.charAt(i);
            Collectible collectible = CollectibleUtils.getCollectible(collectibleChar);
            i++;
            for (int j = 0; j < count; j++) {
                TileType tile = new TileType(terrain, collectible);
                tiles.add(tile);
            }
        }
        return tiles.toArray(new TileType[0]);
    }
}
