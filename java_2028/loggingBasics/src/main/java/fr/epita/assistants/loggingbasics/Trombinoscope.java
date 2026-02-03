package fr.epita.assistants.loggingbasics;

import ch.qos.logback.classic.Level;
import ch.qos.logback.classic.Logger;
import org.slf4j.LoggerFactory;


import java.util.*;

public class Trombinoscope {
    private final Logger LOGGER;
    private final HashMap<String, Long> map;

    public Trombinoscope() {
        // FIXME: Instantiate logger with level TRACE
        LOGGER = (Logger) LoggerFactory.getLogger(Trombinoscope.class);
        LOGGER.setLevel(Level.TRACE);

        // FIXME: Add logging here
        LOGGER.trace("Instantiating new Trombinoscope");

        map = new HashMap<>();
    }

    public Long putPerson(String name, long photoId) {
        // FIXME: Add logging here
        LOGGER.debug("Putting person (\"{}\", {})", name, photoId);

        Long oldPhotoId = map.put(name,
                                  photoId);

        // FIXME: Add logging here
        if (oldPhotoId == null) {
            LOGGER.trace("Added entry for person \"{}\"", name);
        }
        else {
            LOGGER.trace("Updated for person \"{}\"", name);
        }

        return oldPhotoId;
    }
}
