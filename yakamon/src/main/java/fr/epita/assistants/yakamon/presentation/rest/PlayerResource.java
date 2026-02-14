package fr.epita.assistants.yakamon.presentation.rest;

import fr.epita.assistants.yakamon.converter.PlayerConverter;
import fr.epita.assistants.yakamon.domain.entity.PlayerEntity;
import fr.epita.assistants.yakamon.domain.service.PlayerService;
import fr.epita.assistants.yakamon.presentation.api.response.PlayerResponse;
import fr.epita.assistants.yakamon.utils.ErrorInfo;
import jakarta.inject.Inject;
import jakarta.ws.rs.Consumes;
import jakarta.ws.rs.GET;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;

@Path("/player")
@Produces(MediaType.APPLICATION_JSON)
@Consumes(MediaType.APPLICATION_JSON)
public class PlayerResource {

    @Inject
    PlayerService playerService;
    @Inject
    PlayerConverter playerConverter;
    @GET
    public Response getPlayer() {
        if (!playerService.playerExists()) {
            return Response
                    .status(Response.Status.BAD_REQUEST)
                    .entity(new ErrorInfo("The game is not running."))
                    .build();
        }

        PlayerEntity player = playerService.getPlayer();

        PlayerResponse response = playerConverter.toResponse(player);

        return Response
                .ok(response)
                .build();
    }

}
