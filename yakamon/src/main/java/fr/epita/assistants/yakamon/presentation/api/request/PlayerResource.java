package fr.epita.assistants.yakamon.presentation.api.request;
import fr.epita.assistants.yakamon.converter.Playerconverter;
import fr.epita.assistants.yakamon.data.model.PlayerModel;
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
    Playerconverter playerConverter;
    @GET
    public Response getPlayer() {
        if (!playerService.playerExists()) {
            return Response
                    .status(Response.Status.BAD_REQUEST)
                    .entity(new ErrorInfo("The game is not running."))
                    .build();
        }

        PlayerModel player = playerService.getPlayer();

        PlayerResponse response = playerConverter.convertToresponse(player);

        return Response
                .ok(response)
                .build();
    }

}
