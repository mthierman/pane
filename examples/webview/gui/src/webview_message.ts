export type webview_message_type = "init" | "test";

export type webview_message_payload = {
    init: { name: string; age: number };
    test: { one: number; two: number };
};

export type webview_message<T = unknown> = {
    [K in keyof T]: { type: K; payload: T[K] };
}[keyof T];
