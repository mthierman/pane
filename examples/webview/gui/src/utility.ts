export function getElementById<T = HTMLElement>(elementId: string) {
    return document.getElementById(elementId) as T | null;
}

export function getBody() {
    // let body: HTMLBodyElement;
    // body = document.body as HTMLBodyElement;
    // let body = document.body as HTMLBodyElement;
    // return body;

    return document.body as HTMLBodyElement | null;
}
