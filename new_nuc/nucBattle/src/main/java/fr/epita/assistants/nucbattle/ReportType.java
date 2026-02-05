package fr.epita.assistants.nucbattle;

import com.fasterxml.jackson.annotation.JsonValue;

public enum ReportType {
    WINNER("winner"),
    CHEATER("cheater"),
    ERROR("error"),
    UNFINISHED("unfinished");

    private final String value;

    ReportType(String value) {
        this.value = value;
    }

    @JsonValue
    public String getValue() {
        return value;
    }
}

