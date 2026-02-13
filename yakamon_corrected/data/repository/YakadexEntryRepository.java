package fr.epita.assistants.yakamon.data.repository;

import fr.epita.assistants.yakamon.data.model.YakadexentryModel;
import io.quarkus.hibernate.orm.panache.PanacheRepositoryBase;
import jakarta.enterprise.context.ApplicationScoped;

import java.util.List;

@ApplicationScoped
public class YakadexEntryRepository implements PanacheRepositoryBase<YakadexentryModel, Integer> {

    /**
     * ✅ Trouve une entrée par ID
     */
    public YakadexentryModel findById(Integer id) {
        return find("id", id).firstResult();
    }

    /**
     * ✅ Retourne toutes les entrées
     */
    public List<YakadexentryModel> findAllEntries() {
        return listAll();
    }

    /**
     * ✅ Trouve les yakamons non capturés
     */
    public List<YakadexentryModel> findUncaughtEntries() {
        return find("caught", false).list();
    }

    /**
     * ✅ Trouve les yakamons capturés
     */
    public List<YakadexentryModel> findCaughtEntries() {
        return find("caught", true).list();
    }

    /**
     * ✅ Réinitialise tous les yakamons comme non capturés
     */
    public void resetAllCaught() {
        update("caught = false");
    }

    /**
     * ✅ Sauvegarde une entrée
     */
    public void saveEntry(YakadexentryModel entry) {
        persist(entry);
    }
}
