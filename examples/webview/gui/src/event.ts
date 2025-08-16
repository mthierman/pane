export class Event {
    static new<T extends Record<string, any>>(type: keyof T, detail: T[keyof T]) {
        return new CustomEvent(type as string, { detail });
    }
}
