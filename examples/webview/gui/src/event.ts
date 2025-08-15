type event_payload = {
    init: { name: string; age: number };
    test: { one: number; two: number };
};

export type event<T = event_payload> = {
    [K in keyof T]: { type: K; payload: T[K] };
}[keyof T];
