package fr.epita.assistants.yakamon.data.repository;

import fr.epita.assistants.yakamon.data.model.GameModel;
import io.quarkus.hibernate.orm.panache.PanacheRepositoryBase;
import jakarta.enterprise.context.ApplicationScoped;

@ApplicationScoped
public class GameRepository implements PanacheRepositoryBase<GameModel, Integer> {

    /**
     * ✅ Retourne un MODEL (pas une Entity)
     * La conversion Model → Entity se fera dans le Service via le ModelConverter
     */
    public GameModel findGame() {
        return listAll().stream().findFirst().orElse(null);
    }

    /**
     * ✅ Prend et retourne un MODEL
     * La conversion Entity → Model se fait dans le Service via le ModelConverter
     */
    public GameModel saveGame(GameModel model) {
        persist(model);
        return model;
    }

    /**
     * ✅ Supprime tous les jeux
     */
    public void deleteAll() {
        delete("1=1");
    }
}
