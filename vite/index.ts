import "./styles/index.css";

type Data = { message: string };

window.chrome.webview.addEventListener<Data>("message", (event) => {
    const data = event.data;
    document.getElementById("root")!.innerHTML = data.message;
});
