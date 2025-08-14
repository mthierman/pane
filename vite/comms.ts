import { type Button } from "./button";

export type webview_message_type = "init" | "test";

export type payload = {
    init: { name: string; age: number };
    test: { one: number; two: number };
};

export type webview_message = {
    [K in keyof payload]: { webview_message_type: K; payload: payload[K] };
}[keyof payload];

const button = document.getElementById("button") as Button;

window.chrome.webview.addEventListener<webview_message>("message", (event) => {
    const data = event.data;
    console.log(data);
    switch (data.webview_message_type) {
        case "init":
            {
                console.log(data.payload.name);
            }
            break;
        case "test":
            {
                console.log(data.payload.one);
            }
            break;
    }
});
