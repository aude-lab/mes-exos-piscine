package fr.epita.assistants.throwback;

public abstract class IntegerException extends Exception {
    public IntegerException(String message) {
        super("IntegerException: " + message);
    }
}
