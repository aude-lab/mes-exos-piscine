package com.epita.creeps;

import kong.unirest.core.Unirest;
import kong.unirest.core.HttpResponse;
import kong.unirest.core.JsonNode;

public class Program {
    public static void main(String args[]) {
        if (args.length != 3) {
            System.err.println("args number error");
            System.exit(1);
        }

        String host = args[0];
        int port = Integer.parseInt(args[1]);
        String myLogin = args[2];
        String baseUrl = "http://" + host + ":" + port;

        System.out.println("=== BOB THE BUILDER ===");
        System.out.println("Serveur: " + host + ":" + port);
        System.out.println("Login: " + myLogin);
        System.out.println();

        // Connexion
        try {
            HttpResponse<JsonNode> statusResponse = Unirest.get(baseUrl + "/status").asJson();
            if (!statusResponse.getBody().getObject().getBoolean("running")) {
                System.out.println("Serveur non demarre");
                return;
            }
            System.out.println("Serveur OK");
        } catch (Exception e) {
            System.out.println("Erreur serveur");
            return;
        }

        JsonNode initbody = null;
        try {
            HttpResponse<JsonNode> initResponse = Unirest.post(baseUrl + "/init/" + myLogin).asJson();
            initbody = initResponse.getBody();

            if (initbody.getObject().has("error") && !initbody.getObject().isNull("error")) {
                System.out.println("Erreur connexion");
                return;
            }

            System.out.println("Connecte - Hello World!");
        } catch (Exception e) {
            System.out.println("Erreur init");
            return;
        }

        if (initbody == null) {
            return;
        }

        String citizen1Id = initbody.getObject().getString("citizen1Id");
        String citizen2Id = initbody.getObject().getString("citizen2Id");

        System.out.println("Citizens: " + citizen1Id + " et " + citizen2Id);
        System.out.println();

        // =========================================
        // OBJECTIF: Construire une route
        // =========================================

        System.out.println(">>> Objectif: Bob the Builder");
        System.out.println();

        // Etape 1: Se deplacer pour etre sur une tile vide
        System.out.println("Etape 1: Se deplacer");
        String move1 = sendCommand(baseUrl, myLogin, citizen2Id, "move:right");
        if (move1 != null) {
            waitForReport(baseUrl, move1);
            System.out.println("Deplacement OK");
        }

        System.out.println();

        // Etape 2: Construire une route
        System.out.println("Etape 2: Construire une route");
        String build = sendCommand(baseUrl, myLogin, citizen2Id, "build:road");

        if (build != null) {
            waitForReport(baseUrl, build);

            // Verifier le rapport
            JsonNode buildReport = getReport(baseUrl, build);
            if (buildReport != null) {
                String status = buildReport.getObject().getString("status");

                if (status.equals("SUCCESS")) {
                    System.out.println("*** ACHIEVEMENT: Bob the Builder - VALIDE! ***");
                } else {
                    System.out.println("Echec: " + status);
                    if (buildReport.getObject().has("errorCode")) {
                        String errorCode = buildReport.getObject().getString("errorCode");
                        System.out.println("Erreur: " + errorCode);

                        if (errorCode.equals("insufficient-funds")) {
                            System.out.println("Pas assez de ressources");
                            System.out.println("Ressources de depart: wood=30, rock=30");
                            System.out.println("Cout d'une route: environ wood=5, rock=5");
                        }
                        if (errorCode.equals("tile-occupied")) {
                            System.out.println("La tile n'est pas vide");
                            System.out.println("Essaye de te deplacer encore");
                        }
                    }
                }
            }
        } else {
            System.out.println("Commande build echouee");
        }

        System.out.println();
        System.out.println("=== FIN ===");
        System.out.println("Voir: http://localhost:1337");
    }

    private static String sendCommand(String baseUrl, String login, String unitId, String opcode) {
        try {
            String url = baseUrl + "/command/" + login + "/" + unitId + "/" + opcode;
            HttpResponse<JsonNode> response = Unirest.post(url).body("{}").asJson();
            JsonNode body = response.getBody();

            if (body.getObject().has("errorCode") && !body.getObject().isNull("errorCode")) {
                String errorCode = body.getObject().getString("errorCode");
                if (!errorCode.isEmpty()) {
                    System.out.println("Erreur commande: " + errorCode);
                }
                return null;
            }

            return body.getObject().getString("reportId");
        } catch (Exception e) {
            System.out.println("Exception: " + e.getMessage());
            return null;
        }
    }

    private static void waitForReport(String baseUrl, String reportId) {
        int attempts = 0;
        while (attempts < 1000) {
            attempts++;
            try {
                HttpResponse<JsonNode> response = Unirest.get(baseUrl + "/report/" + reportId).asJson();
                String opcode = response.getBody().getObject().getString("opcode");
                if (!opcode.equals("noreport")) {
                    return;
                }
            } catch (Exception e) {
                return;
            }
        }
    }

    private static JsonNode getReport(String baseUrl, String reportId) {
        try {
            HttpResponse<JsonNode> response = Unirest.get(baseUrl + "/report/" + reportId).asJson();
            return response.getBody();
        } catch (Exception e) {
            return null;
        }
    }
}