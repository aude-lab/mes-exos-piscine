package fr.epita.assistants.throwback;

public class PositiveIntegerException extends IntegerException {
    public PositiveIntegerException(String message) {
        super("PositiveIntegerException: " + message);
    }
}
