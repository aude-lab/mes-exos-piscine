{
  description = "Flake for libzork";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }: {
    devShells.x86_64-linux.default = with nixpkgs.legacyPackages.x86_64-linux; mkShell {
      buildInputs = [
        gcc
        cmake
        yaml-cpp
        curl
        nlohmann_json
        openssl
        cmake
        pkg-config
      ];

    };
  };
}
