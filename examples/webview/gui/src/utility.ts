export const Utility = {
    html: document.documentElement as HTMLHtmlElement,
    body: document.body as HTMLBodyElement,

    getElementById<T = HTMLElement>(elementId: string) {
        return document.getElementById(elementId) as T | null;
    },

    customEvent<T>(event: WebViewMessageEvent<WebViewMessageEventData<T>>) {
        return new CustomEvent(event.data.type as string, { detail: event.data.payload });
    },
};
