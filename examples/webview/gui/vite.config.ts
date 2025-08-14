import { type CommonServerOptions, type UserConfig, defineConfig } from "vite";
import oxlintPlugin from "vite-plugin-oxlint";

// https://vitejs.dev/config/
export default defineConfig(({ command }) => {
    const userConfig: UserConfig = {
        base: "./",
        plugins: [oxlintPlugin()],
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
