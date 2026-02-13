package fr.epita.assistants.yakamon.data.repository;

import fr.epita.assistants.yakamon.data.model.PlayerModel;
import io.quarkus.hibernate.orm.panache.PanacheRepositoryBase;
import jakarta.enterprise.context.ApplicationScoped;

import java.util.UUID;

@ApplicationScoped
public class PlayerRepository implements PanacheRepositoryBase<PlayerModel, UUID> {

    /**
     * ✅ Retourne un MODEL (pas une Entity)
     * ❌ AVANT : injectait ModelConverter et retournait une Entity
     */
    public PlayerModel findPlayer() {
        return listAll().stream().findFirst().orElse(null);
    }

    /**
     * ✅ Prend et retourne un MODEL
     * La conversion Entity → Model se fait dans le Service
     */
    public PlayerModel savePlayer(PlayerModel model) {
        persist(model);
        return model;
    }

    /**
     * ✅ Supprime tous les joueurs
     */
    public void deleteAll() {
        delete("1=1");
    }
}
