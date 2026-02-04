package fr.epita.assistants.fgen;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Path;
import java.nio.file.Paths;

public class FGen {
    
    private Path currentPath;

    public FGen(final String workingDirectoryPath) {
        this.currentPath = Paths.get(workingDirectoryPath).toAbsolutePath().normalize();
        
        File workDir = currentPath.toFile();
        if (!workDir.exists()) {
            workDir.mkdirs();
        }
    }

    public void run(final String scriptPath) {
        try (InputStream inputStream = ClassLoader.getSystemResourceAsStream(scriptPath)) {
            if (inputStream == null) {
                throw new RuntimeException("Script file not found: " + scriptPath);
            }
            
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream))) {
                String line;
                while ((line = reader.readLine()) != null) {
                    if (line.trim().isEmpty()) {
                        continue;
                    }
                    
                    executeCommand(line);
                }
            }
        } catch (IOException e) {
            throw new RuntimeException("Error reading script: " + scriptPath, e);
        }
    }
    
    private void executeCommand(String line) {
        if (line.length() < 3) {
            throw new RuntimeException("Invalid command: " + line);
        }
        
        char opcode = line.charAt(0);
        String path = line.substring(2);
        
        switch (opcode) {
            case '+':
                createPath(path);
                break;
            case '>':
                changePath(path);
                break;
            case '-':
                deletePath(path);
                break;
            default:
                throw new RuntimeException("Unknown opcode: " + opcode);
        }
    }
    
    private void createPath(String relativePath) {
        Path targetPath = currentPath.resolve(relativePath).normalize();
        File target = targetPath.toFile();
        
        try {
            if (relativePath.endsWith("/")) {
                if (!target.exists()) {
                    target.mkdirs();
                }
            } else {
                File parent = target.getParentFile();
                if (parent != null && !parent.exists()) {
                    parent.mkdirs();
                }
                
                if (!target.exists()) {
                    target.createNewFile();
                }
            }
        } catch (IOException e) {
            throw new RuntimeException("Error creating: " + relativePath, e);
        }
    }
    
    private void changePath(String relativePath) {
        Path newPath = currentPath.resolve(relativePath).normalize();
        
        File newDir = newPath.toFile();
        if (!newDir.exists() || !newDir.isDirectory()) {
            throw new RuntimeException("Invalid directory: " + relativePath);
        }
        
        currentPath = newPath;
    }
    
    private void deletePath(String relativePath) {
        Path targetPath = currentPath.resolve(relativePath).normalize();
        File target = targetPath.toFile();
        
        if (!target.exists()) {
            return;
        }
        
        deleteRecursively(target);
    }
    
    private void deleteRecursively(File file) {
        if (file.isDirectory()) {
            File[] contents = file.listFiles();
            if (contents != null) {
                for (File child : contents) {
                    deleteRecursively(child);
                }
            }
        }
        
        file.delete();
    }
}
