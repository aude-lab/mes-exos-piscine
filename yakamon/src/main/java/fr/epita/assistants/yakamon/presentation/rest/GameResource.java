package fr.epita.assistants.yakamon.presentation.rest;
import fr.epita.assistants.yakamon.converter.GameConverter;
import fr.epita.assistants.yakamon.data.model.GameModel;
import fr.epita.assistants.yakamon.domain.service.*;
import fr.epita.assistants.yakamon.presentation.api.request.StartRequest;
import fr.epita.assistants.yakamon.presentation.api.response.StartResponse;
import fr.epita.assistants.yakamon.utils.ErrorInfo;
import fr.epita.assistants.yakamon.utils.tile.ItemType;
import jakarta.inject.Inject;
import jakarta.ws.rs.*;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;

import java.nio.file.Path;

import java.io.IOException;
import java.nio.file.Files;


@jakarta.ws.rs.Path("/start")
@Produces(MediaType.APPLICATION_JSON)
@Consumes(MediaType.APPLICATION_JSON)
public class GameResource {

    @Inject
    GameService gameService;

    @Inject
    PlayerService playerService;

    @Inject
    ItemService itemService;

    @Inject
    YakadexEntryService yakadexEntryService;

    @Inject
    GameConverter gameConverter;

    @POST
    public Response startGame(StartRequest request) {
        if (request.getPlayerName() == null || request.getPlayerName().isBlank()) {
            return Response
                    .status(Response.Status.BAD_REQUEST)
                    .entity(new ErrorInfo("Invalid name"))
                    .build();
        }
        if (request.getPlayerName().length() > 20) {
            return Response
                    .status(Response.Status.BAD_REQUEST)
                    .entity(new ErrorInfo("length invalid"))
                    .build();
        }
        if (request.getMapPath() == null || request.getMapPath().isBlank()) {
            return Response
                    .status(Response.Status.BAD_REQUEST)
                    .entity(new ErrorInfo("Invalid path"))
                    .build();
        }
        Path mapPath = Path.of(request.getMapPath());
        if (!Files.exists(mapPath)) {
            return Response
                    .status(Response.Status.BAD_REQUEST)
                    .entity(new ErrorInfo("Invalid path"))
                    .build();
        }

        playerService.deletedAll();
        itemService.deleteAll();
        gameService.deletedAll();

        yakadexEntryService.resetAllCaught();

        playerService.createPlayer(request.getPlayerName());

        itemService.addItem(ItemType.YAKABALL, 5);

        GameModel game;
        try {
            game = gameService.createGame(request.getMapPath());
        } catch (IOException e) {
            return Response
                    .status(Response.Status.BAD_REQUEST)
                    .entity(new ErrorInfo("Error"))
                    .build();
        }

        StartResponse response = gameConverter.toStartResponse(game);

        return Response
                .ok(response)
                .build();
    }

}
