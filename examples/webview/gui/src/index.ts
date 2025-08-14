import "./index.css";

export type webview_message_type = "init" | "test";

export type webview_message_payload = {
    init: { name: string; age: number };
    test: { one: number; two: number };
};

export type webview_message<T = unknown> = {
    [K in keyof T]: { type: K; payload: T[K] };
}[keyof T];

// const button = document.getElementById("button") as Button;

window.chrome.webview.addEventListener<webview_message<webview_message_payload>>(
    "message",
    (event) => {
        const data = event.data;
        console.log(data);
        switch (data.type) {
            case "init":
                {
                    console.log(data.payload.name);
                }
                break;
            case "test":
                {
                    console.log(data.payload.two);
                }
                break;
        }
    },
);
