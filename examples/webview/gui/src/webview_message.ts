type event_payload = {
    init: { name: string; age: number };
    test: { one: number; two: number };
};

export type webview_message<T = event_payload> = {
    [K in keyof T]: { type: K; payload: T[K] };
}[keyof T];
