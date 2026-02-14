package fr.epita.assistants.yakamon.domain.service;

import fr.epita.assistants.yakamon.converter.ModelConverter;
import fr.epita.assistants.yakamon.data.model.YakadexentryModel;
import fr.epita.assistants.yakamon.data.repository.YakadexEntryRepository;
import fr.epita.assistants.yakamon.domain.entity.YakadexEntryEntity;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;

import java.util.List;

@ApplicationScoped
public class YakadexEntryService {

    @Inject
    YakadexEntryRepository yakadexRepository;

    @Inject
    ModelConverter modelConverter;


    public List<YakadexEntryEntity> getAllEntries() {
        List<YakadexentryModel> models = yakadexRepository.findAllEntries();
        return modelConverter.toYakadexEntryEntityList(models);
    }


    public List<YakadexEntryEntity> getUncaughtEntries() {
        List<YakadexentryModel> models = yakadexRepository.findUncaughtEntries();
        return modelConverter.toYakadexEntryEntityList(models);
    }


    public YakadexEntryEntity getEntryById(Integer id) {
        YakadexentryModel model = yakadexRepository.findById(id);
        return modelConverter.toYakadexEntryEntity(model);
    }


    @Transactional
    public void markAsCaught(Integer yakadexId) {
        YakadexentryModel model = yakadexRepository.findById(yakadexId);

        if (model != null) {
            model.setCaught(true);
            yakadexRepository.saveEntry(model);
        }
    }


    @Transactional
    public void resetAllCaught() {
        yakadexRepository.resetAllCaught();
    }
}
