package fr.epita.assistants.embedfiles;

import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.Optional;

public class DisplayEmbedFile {
    private final String filename;

    public DisplayEmbedFile(String filename) {
        this.filename = filename;
    }

    public Optional<String> display() {
        ClassLoader classLoader = getClass().getClassLoader();
        try (InputStream stream = classLoader.getResourceAsStream(filename)) {
            if (stream == null) {
                return Optional.empty();
            }

            String content = new String(stream.readAllBytes(), StandardCharsets.UTF_8);
            return Optional.of(content);
        } catch (IOException e) {
            return Optional.empty();

        }
    }
}
