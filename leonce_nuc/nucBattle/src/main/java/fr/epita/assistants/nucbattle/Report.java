package fr.epita.assistants.nucbattle;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.*;
import java.util.Map;

@Getter
@JsonInclude(JsonInclude.Include.NON_NULL) // Ignore null fields during JSON serialization
public class Report {
    @JsonProperty("type")
    private ReportType reportType;

    @JsonProperty("player")
    private String player;

    @JsonProperty("outcome")
    private Map<String, Float> outcome;

    // For the error type report
    public Report(ReportType reportType) {
        this.reportType = reportType;
    }

    // For cheater type report
    public Report(ReportType reportType, String player) {
        this.reportType = reportType;
        this.player = player;
    }

    // For the unfinished type report
    public Report(ReportType reportType, Map<String, Float> outcome) {
        this.reportType = reportType;
        this.outcome = outcome;
    }

    // For the winner type report
    public Report(ReportType reportType, String player, Map<String, Float> outcome) {
        this.reportType = reportType;
        this.player = player;
        this.outcome = outcome;
    }
}
