import "./styles/index.css";

type Data = { message: string };

const root = document.getElementById("root");

window.chrome.webview.addEventListener<Data>("message", (event) => {
    const data = event.data;

    if (root) {
        root.innerHTML = data.message;
    }
});
