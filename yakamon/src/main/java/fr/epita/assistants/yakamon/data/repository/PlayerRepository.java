
package fr.epita.assistants.yakamon.data.repository;

import fr.epita.assistants.yakamon.data.model.PlayerModel;
import io.quarkus.hibernate.orm.panache.PanacheRepositoryBase;
import jakarta.enterprise.context.ApplicationScoped;
import java.util.UUID;

@ApplicationScoped
public class PlayerRepository implements PanacheRepositoryBase<PlayerModel, UUID> {

    public PlayerModel findPlayer() {
        return listAll().stream().findFirst().orElse(null);
    }

    public void savePlayer(PlayerModel player) {
        persist(player);
    }

    public void deletedAll() {
        delete("true");
    }
}
