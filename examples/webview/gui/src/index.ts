import { Button } from "./button";
import "./index.css";

Button.define();
// const button = Button.getElementById("button");

export type payload = {
    init: { name: string; age: number };
    test: { one: number; two: number };
};

window.chrome.webview.addEventListener<payload>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                console.log(event.data);
            }
            break;
        case "test":
            {
                console.log(event.data);
            }
            break;
    }
});
