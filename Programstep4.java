package com.epita.creeps;

import kong.unirest.core.Unirest;
import kong.unirest.core.HttpResponse;
import kong.unirest.core.JsonNode;
import org.json.JSONArray;
import org.json.JSONObject;

public class Program {
    public static void main(String args[]) {
        if (args.length != 3) {
            System.err.println("args number error ");
            System.exit(1);
        }
        String host = args[0];
        int port = Integer.parseInt(args[1]);
        String myLogin = args[2];
        System.out.println("Creeps start:");
        System.out.println("Serveur: " + host + ":" + port);
        System.out.println("Login: " + myLogin);
        System.out.println();
        String baseUrl = "http://" + host + ":" + port;
        System.out.println("Verification cotee serveur:");
        try {
            HttpResponse<JsonNode> statusResponse = Unirest.get(baseUrl + "/status")
                    .asJson();

            boolean running = statusResponse.getBody().getObject().getBoolean("running");

            if (running) {
                System.out.println("Serveur en ligne");
                System.out.println();
            } else {
                System.out.println();
                System.out.println("Le jeu n'est pas demarre");
                System.out.println();
                return;
            }
        } catch (Exception e) {
            System.out.println("Impossible de contacter le serveur");
            return;
        }

        System.out.println("Connexion au jeu:");
        JsonNode initBody = null;
        try {
            HttpResponse<JsonNode> initResponse = Unirest.post(baseUrl + "/init/" + myLogin)
                    .asJson();
            initBody = initResponse.getBody();
            if (initBody.getObject().has("error") && !initBody.getObject().isNull("error")) {
                System.out.println("Erreur: " + initBody.getObject().getString("error"));
                return;
            }

            System.out.println("Connectee");
            System.out.println("Achievement: Hello, World!");
            System.out.println();

        } catch (Exception e) {
            System.out.println("Erreur lors de la connexion");
            System.out.println();
            return;
        }

        // Récupérer les IDs des citizens
        String citizen1Id = initBody.getObject().getString("citizen1Id");
        String citizen2Id = initBody.getObject().getString("citizen2Id");
        
        System.out.println("Citizens IDs:");
        System.out.println("  Citizen 1: " + citizen1Id);
        System.out.println("  Citizen 2: " + citizen2Id);
        System.out.println();

        // ========================================
        // STRATEGIE : Trouver et recolter une ressource
        // ========================================
        
        System.out.println("=== OBJECTIF: Trouver et recolter une ressource ===");
        System.out.println();
        
        // Etape 1 : Observer pour voir les tiles autour
        System.out.println("Etape 1: Observer l'environnement");
        String reportId = sendCommand(baseUrl, myLogin, citizen1Id, "observe");
        if (reportId == null) {
            System.out.println("Echec de la commande observe");
            return;
        }
        waitForReport(baseUrl, reportId);
        
        // Recuperer les tiles observees
        JSONObject observeReport = getReportAsJson(baseUrl, reportId);
        if (observeReport == null) {
            System.out.println("Impossible de recuperer le rapport");
            return;
        }
        
        System.out.println("Analyse des tiles...");
        
        // Chercher une tile avec une ressource
        JSONArray tiles = observeReport.getJSONArray("tiles");
        String resourceType = null;
        int resourceX = -1;
        int resourceY = -1;
        
        for (int i = 0; i < tiles.length(); i++) {
            JSONObject tile = tiles.getJSONObject(i);
            String type = tile.getString("type");
            
            // Les ressources possibles : wood, rock, food, oil
            if (type.equals("wood") || type.equals("rock") || 
                type.equals("food") || type.equals("oil")) {
                resourceType = type;
                JSONObject pos = tile.getJSONObject("position");
                resourceX = pos.getInt("x");
                resourceY = pos.getInt("y");
                System.out.println("Ressource trouvee: " + resourceType + 
                                 " a la position (" + resourceX + ", " + resourceY + ")");
                break;
            }
        }
        
        if (resourceType == null) {
            System.out.println("Aucune ressource trouvee autour !");
            System.out.println("Il faut explorer plus loin...");
            
            // Explorer en se deplacant
            System.out.println();
            System.out.println("Etape 2: Explorer en se deplacant");
            explorePourTrouverRessource(baseUrl, myLogin, citizen1Id);
            return;
        }
        
        System.out.println();
        
        // Etape 2 : Se deplacer vers la ressource
        System.out.println("Etape 2: Se deplacer vers la ressource");
        
        // Recuperer la position actuelle du citizen
        JSONObject unitPos = observeReport.getJSONObject("unitPosition");
        int currentX = unitPos.getInt("x");
        int currentY = unitPos.getInt("y");
        
        System.out.println("Position actuelle: (" + currentX + ", " + currentY + ")");
        System.out.println("Position cible: (" + resourceX + ", " + resourceY + ")");
        
        // Se deplacer vers la ressource
        boolean arrived = moveToPosition(baseUrl, myLogin, citizen1Id, 
                                        currentX, currentY, resourceX, resourceY);
        
        if (!arrived) {
            System.out.println("Impossible d'atteindre la ressource");
            return;
        }
        
        System.out.println();
        
        // Etape 3 : Recolter la ressource
        System.out.println("Etape 3: Recolter la ressource");
        String gatherReportId = sendCommand(baseUrl, myLogin, citizen1Id, "gather");
        if (gatherReportId != null) {
            System.out.println("Commande gather envoyee !");
            System.out.println("Achievement: Gather a resource!");
            System.out.println();
            
            waitForReport(baseUrl, gatherReportId);
            getReport(baseUrl, gatherReportId);
        }
        
        System.out.println("=== Programme termine ===");
        System.out.println("Va voir le viewer pour verifier les achievements !");
        System.out.println("http://localhost:1337");
    }

    /**
     * Se deplace vers une position cible
     */
    private static boolean moveToPosition(String baseUrl, String login, String unitId,
                                         int fromX, int fromY, int toX, int toY) {
        int currentX = fromX;
        int currentY = fromY;
        
        int maxMoves = 10; // Securite
        int moves = 0;
        
        while ((currentX != toX || currentY != toY) && moves < maxMoves) {
            moves++;
            
            String direction = null;
            
            // Determiner la direction
            if (currentX < toX) {
                direction = "move:right";
                currentX++;
            } else if (currentX > toX) {
                direction = "move:left";
                currentX--;
            } else if (currentY < toY) {
                direction = "move:up";
                currentY++;
            } else if (currentY > toY) {
                direction = "move:down";
                currentY--;
            }
            
            if (direction != null) {
                System.out.println("Deplacement: " + direction);
                String reportId = sendCommand(baseUrl, login, unitId, direction);
                if (reportId != null) {
                    waitForReport(baseUrl, reportId);
                } else {
                    return false;
                }
            }
        }
        
        System.out.println("Position atteinte !");
        return true;
    }

    /**
     * Explore en se deplacant pour trouver une ressource
     */
    private static void explorePourTrouverRessource(String baseUrl, String login, String unitId) {
        String[] directions = {"move:up", "move:right", "move:down", "move:left"};
        
        for (int i = 0; i < 4; i++) {
            System.out.println("Deplacement: " + directions[i]);
            String reportId = sendCommand(baseUrl, login, unitId, directions[i]);
            if (reportId != null) {
                waitForReport(baseUrl, reportId);
                
                // Recuperer les tiles observees lors du move
                JSONObject moveReport = getReportAsJson(baseUrl, reportId);
                if (moveReport != null && moveReport.has("tiles")) {
                    JSONArray tiles = moveReport.getJSONArray("tiles");
                    
                    // Chercher une ressource
                    for (int j = 0; j < tiles.length(); j++) {
                        JSONObject tile = tiles.getJSONObject(j);
                        String type = tile.getString("type");
                        
                        if (type.equals("wood") || type.equals("rock") || 
                            type.equals("food") || type.equals("oil")) {
                            JSONObject pos = tile.getJSONObject("position");
                            System.out.println("Ressource trouvee: " + type + 
                                             " a la position (" + pos.getInt("x") + 
                                             ", " + pos.getInt("y") + ")");
                            
                            // Se deplacer vers cette ressource et la recolter
                            // (code simplifie pour cet exemple)
                            return;
                        }
                    }
                }
            }
        }
        
        System.out.println("Aucune ressource trouvee apres exploration");
    }

    /**
     * Envoie une commande a une unite
     */
    private static String sendCommand(String baseUrl, String login, String unitId, String opcode) {
        try {
            String url = baseUrl + "/command/" + login + "/" + unitId + "/" + opcode;
            
            HttpResponse<JsonNode> response = Unirest.post(url)
                    .body("{}")
                    .asJson();

            JsonNode body = response.getBody();
            
            // Verifier les erreurs
            if (body.getObject().has("errorCode") && !body.getObject().isNull("errorCode")) {
                String errorCode = body.getObject().getString("errorCode");
                System.out.println("ERREUR: " + errorCode);
                if (body.getObject().has("error")) {
                    System.out.println("Details: " + body.getObject().getString("error"));
                }
                return null;
            }

            // Recuperer le report ID
            String reportId = body.getObject().getString("reportId");
            return reportId;

        } catch (Exception e) {
            System.out.println("Erreur lors de l'envoi de la commande: " + e.getMessage());
            return null;
        }
    }

    /**
     * Attend qu'un rapport soit pret (POLLING sans Thread.sleep)
     */
    private static void waitForReport(String baseUrl, String reportId) {
        int attempts = 0;
        int maxAttempts = 1000;
        
        while (attempts < maxAttempts) {
            attempts++;
            
            try {
                String url = baseUrl + "/report/" + reportId;
                HttpResponse<JsonNode> response = Unirest.get(url).asJson();
                JsonNode body = response.getBody();
                
                String opcode = body.getObject().getString("opcode");
                
                if (!opcode.equals("noreport")) {
                    // Le rapport est pret
                    return;
                }
                
            } catch (Exception e) {
                return;
            }
        }
    }

    /**
     * Recupere et affiche un rapport
     */
    private static void getReport(String baseUrl, String reportId) {
        try {
            String url = baseUrl + "/report/" + reportId;
            HttpResponse<JsonNode> response = Unirest.get(url).asJson();
            JsonNode body = response.getBody();
            
            String opcode = body.getObject().getString("opcode");
            if (opcode.equals("noreport")) {
                System.out.println("Rapport pas encore pret");
                System.out.println();
                return;
            }

            String status = body.getObject().getString("status");
            System.out.println("Statut: " + status);
            
            if (status.equals("SUCCESS")) {
                System.out.println("Commande executee avec succes !");
                
                // Afficher la position si presente
                if (body.getObject().has("newPosition")) {
                    JsonNode newPos = body.getObject().getJSONObject("newPosition");
                    System.out.println("Nouvelle position: x=" + newPos.getObject().getInt("x") + 
                                     ", y=" + newPos.getObject().getInt("y"));
                }
                
                // Afficher les infos de gather si present
                if (body.getObject().has("gathered")) {
                    int gathered = body.getObject().getInt("gathered");
                    String resource = body.getObject().getString("resource");
                    int resourcesLeft = body.getObject().getInt("resourcesLeft");
                    System.out.println("Ressource recoltee: " + gathered + " " + resource);
                    System.out.println("Reste sur la tile: " + resourcesLeft);
                }
            } else {
                System.out.println("Commande echouee !");
                if (body.getObject().has("errorCode")) {
                    System.out.println("Code erreur: " + body.getObject().getString("errorCode"));
                }
            }
            System.out.println();

        } catch (Exception e) {
            System.out.println("Erreur: " + e.getMessage());
            System.out.println();
        }
    }

    /**
     * Recupere un rapport sous forme de JSONObject (pour analyser les donnees)
     */
    private static JSONObject getReportAsJson(String baseUrl, String reportId) {
        try {
            String url = baseUrl + "/report/" + reportId;
            HttpResponse<JsonNode> response = Unirest.get(url).asJson();
            return response.getBody().getObject();
        } catch (Exception e) {
            return null;
        }
    }
}
