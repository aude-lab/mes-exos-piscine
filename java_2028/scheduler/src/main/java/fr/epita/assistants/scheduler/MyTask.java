package fr.epita.assistants.scheduler;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;
import java.util.function.Supplier;

public class MyTask<RETURN_TYPE> implements Task<RETURN_TYPE> {
    
    /* FIXME */
    private final CompletableFuture<RETURN_TYPE> future;
    private MyTask(CompletableFuture<RETURN_TYPE> future) {
        this.future = future;
    }

    /* FIXME */

    public static <RETURN_TYPE> Task<RETURN_TYPE> of(Supplier<RETURN_TYPE> actionSupplier) {
        CompletableFuture<RETURN_TYPE> future = CompletableFuture.supplyAsync(actionSupplier);
        return new MyTask<>(future);
    }

    @Override
    public CompletableFuture<RETURN_TYPE> build() {
        return future;
    }

    @Override
    public Task<RETURN_TYPE> onErrorRecoverWith(Function<Throwable, RETURN_TYPE> recoveryFunction) {
        CompletableFuture<RETURN_TYPE> handled = future.handle((result, exception) -> {
            if (exception != null) {
                return recoveryFunction.apply(exception);
            }
            return result;
        });
        return new MyTask<>(handled);
    }

    @Override
    public <NEW_RETURN_TYPE> Task<NEW_RETURN_TYPE> andThenDo(Function<RETURN_TYPE, NEW_RETURN_TYPE> action) {
        CompletableFuture<NEW_RETURN_TYPE> transformed = future.thenApply(action);
        return new MyTask<>(transformed);
    }

    @Override
    public Task<RETURN_TYPE> andThenWait(long number, TimeUnit timeUnit) {
        CompletableFuture<RETURN_TYPE> delayed = future.thenCompose(result ->
                CompletableFuture.supplyAsync(
                        () -> result,
                        CompletableFuture.delayedExecutor(number, timeUnit)
                )
        );
        return new MyTask<>(delayed);
    }
}
