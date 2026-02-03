package fr.epita.assistants.drawing;

public abstract class Entity {
    public static long SEQUENCE = 0;
    public final long id;

    public Entity() {
        this.id = SEQUENCE++;
    }

    public long getId() {
        return id;
    }
}
