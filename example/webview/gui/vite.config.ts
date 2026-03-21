import { type CommonServerOptions, type UserConfig, defineConfig } from "vite";
import { viteSingleFile } from "vite-plugin-singlefile";

// https://vitejs.dev/config/
export default defineConfig(({ command }) => {
    const userConfig: UserConfig = {
        base: "./",
        plugins: [viteSingleFile()],
    };

    const commonServerOptions: CommonServerOptions = {};

    switch (command) {
        case "serve": {
            return {
                ...userConfig,
                server: { ...commonServerOptions },
                preview: { ...commonServerOptions },
            };
        }
        case "build": {
            return {
                ...userConfig,
            };
        }
    }
});
