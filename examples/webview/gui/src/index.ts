import { Button, getElementById } from "./button";
import "./index.css";

Button.define();
const button = getElementById<Button>("button");

export type payload = {
    init: { name: string; age: number };
    test: { one: number; two: number };
};

window.chrome.webview.addEventListener<payload>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                console.log(event.data);

                if (button) {
                    button.innerText = event.data.payload.name;
                }
            }
            break;
        case "test":
            {
                console.log(event.data);
            }
            break;
    }
});
