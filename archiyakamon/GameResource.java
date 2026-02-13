package fr.epita.assistants.yakamon.presentation.rest;

import fr.epita.assistants.yakamon.converter.GameConverter;
import fr.epita.assistants.yakamon.domain.entity.GameEntity;
import fr.epita.assistants.yakamon.domain.service.GameService;
import fr.epita.assistants.yakamon.presentation.api.request.StartRequest;
import fr.epita.assistants.yakamon.presentation.api.response.StartResponse;
import fr.epita.assistants.yakamon.utils.ErrorInfo;
import jakarta.inject.Inject;
import jakarta.ws.rs.*;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

@Path("/start")
@Produces(MediaType.APPLICATION_JSON)
@Consumes(MediaType.APPLICATION_JSON)
public class GameResource {

    @Inject
    GameService gameService;

    @Inject
    GameConverter gameConverter;

    @POST
    public Response startGame(StartRequest request) {
        if (request.getPlayerName() == null || request.getPlayerName().isBlank()) {
            return Response
                .status(Response.Status.BAD_REQUEST)
                .entity(new ErrorInfo("Invalid `name` provided."))
                .build();
        }

        if (request.getPlayerName().length() > 20) {
            return Response
                .status(Response.Status.BAD_REQUEST)
                .entity(new ErrorInfo("Invalid `name` provided."))
                .build();
        }

        if (request.getMapPath() == null || request.getMapPath().isBlank()) {
            return Response
                .status(Response.Status.BAD_REQUEST)
                .entity(new ErrorInfo("Invalid `path` provided."))
                .build();
        }

        Path mapPath = Path.of(request.getMapPath());
        if (!Files.exists(mapPath)) {
            return Response
                .status(Response.Status.BAD_REQUEST)
                .entity(new ErrorInfo("Invalid `path` provided."))
                .build();
        }

        
        GameEntity game;
        try {
            game = gameService.startNewGame(
                request.getPlayerName(), 
                request.getMapPath()
            );
        } catch (IOException e) {
            return Response
                .status(Response.Status.BAD_REQUEST)
                .entity(new ErrorInfo("Invalid `path` provided."))
                .build();
        }

        
        StartResponse response = gameConverter.toStartResponse(game);
        return Response.ok(response).build();
    }
}
