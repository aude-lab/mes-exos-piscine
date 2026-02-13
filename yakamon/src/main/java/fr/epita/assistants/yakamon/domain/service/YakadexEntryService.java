package fr.epita.assistants.yakamon.domain.service;
import fr.epita.assistants.yakamon.data.model.YakadexentryModel;
import fr.epita.assistants.yakamon.data.repository.YakadexEntryRepository;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;

import java.util.List;
@ApplicationScoped

public class YakadexEntryService {

    @Inject
    YakadexEntryRepository yakadexRepository;
    public List<YakadexentryModel> getAllEntries() {
        return yakadexRepository.findAllEntries();
    }

    public List<YakadexentryModel> getUncaughtEntries() {
        return yakadexRepository.findUncaughtEntries();
    }

    public YakadexentryModel  getEntryById(Integer id) {
        return yakadexRepository.findById(id);
    }

    @Transactional
    public void markAsCaught(Integer yakadexId) {
        YakadexentryModel entry = yakadexRepository.findById(yakadexId);

        if (entry != null) {
            entry.setCaught(true);
            yakadexRepository.saveEntry(entry);
        }
    }

    @Transactional
    public void resetAllCaught() {
        yakadexRepository.resetAllCaught();
    }
}
