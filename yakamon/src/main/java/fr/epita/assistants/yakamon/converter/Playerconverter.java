package fr.epita.assistants.yakamon.converter;
import fr.epita.assistants.yakamon.data.model.PlayerModel;
import fr.epita.assistants.yakamon.presentation.api.response.PlayerResponse;
import jakarta.enterprise.context.ApplicationScoped;
@ApplicationScoped

public class Playerconverter {
    public PlayerResponse convertToresponse(PlayerModel model) {
        return new PlayerResponse(
                model.getUuid(),
                model.getName(),
                model.getPosX(),
                model.getPosY(),
                model.getLastMove(),
                model.getLastCollect(),
                model.getLastCatch(),
                model.getLastFeed()

        );
    }
}
