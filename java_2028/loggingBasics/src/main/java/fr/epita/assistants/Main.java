package fr.epita.assistants;

import fr.epita.assistants.loggingbasics.*;
import ch.qos.logback.classic.Level;
import ch.qos.logback.classic.Logger;
import org.slf4j.LoggerFactory;


public class Main {
    private final static Logger logger = (Logger) LoggerFactory.getLogger(Main.class);

    public static void main(String[] args) {
        // FIXME: Add logging here
        logger.setLevel(Level.INFO);
        logger.info( "Entering application" );

        Trombinoscope trombi = new Trombinoscope();

        trombi.putPerson("Bob", 1);
        trombi.putPerson("Bob", 2);
    }
}
