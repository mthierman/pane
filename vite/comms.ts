import { type Button } from "./button";

export type init = { type: "init"; payload: { name: string; age: number } };

type WebViewMessage = init;

const button = document.getElementById("button") as Button;

window.chrome.webview.addEventListener<WebViewMessage>("message", (event) => {
    const data = event.data;
    console.log(data);
    switch (data.type) {
        case "init":
            {
                console.log(data.payload.age);
                button.textContent = data.payload.name;
            }
            break;
    }
});
