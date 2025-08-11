import "./styles/index.css";

type DataOne = { type: "init"; payload: { name: string; age: number } };
type DataTwo = { type: "test"; payload: { test_msg: string; test_number: number } };

type WebViewMessage = DataOne | DataTwo;

const root = document.getElementById("root") as HTMLDivElement;

window.chrome.webview.addEventListener<WebViewMessage>("message", (event) => {
    const data = event.data;
    switch (data.type) {
        case "init":
            {
                console.log(data.payload.age);
                root.innerHTML = data.payload.name;
            }
            break;
        case "test":
            {
                console.log(data.payload.test_number);
                root.innerHTML = data.payload.test_number.toString();
            }
            break;
    }
});
