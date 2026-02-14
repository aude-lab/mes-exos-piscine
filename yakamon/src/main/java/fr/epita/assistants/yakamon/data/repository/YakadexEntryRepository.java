package fr.epita.assistants.yakamon.data.repository;
import fr.epita.assistants.yakamon.data.model.YakadexentryModel;
import io.quarkus.hibernate.orm.panache.PanacheRepositoryBase;
import jakarta.enterprise.context.ApplicationScoped;
import java.util.List;

@ApplicationScoped
public class YakadexEntryRepository implements PanacheRepositoryBase<YakadexentryModel, Integer> {

    public YakadexentryModel findById(Integer id) {
        return find("id", id).firstResult();
    }
    public List<YakadexentryModel> findAllEntries() {
        return listAll();
    }
    public List<YakadexentryModel> findUncaughtEntries() {
        return find("caught", false).list();
    }

    public List<YakadexentryModel> findCaughtEntries() {
        return find("caught", true).list();
    }
    public void resetAllCaught() {
        update("caught = false");
    }
    public void saveEntry(YakadexentryModel entry) {
        persist(entry);
    }
}
