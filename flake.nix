{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs, ... }: let
    systems = [ "x86_64-linux" "aarch64" ];

    forEachSystem = f: with nixpkgs.lib; foldAttrs mergeAttrs {}
      (lists.forEach systems (system:
        mapAttrs (name: value: { ${system} = value; }) (f system)
      ));

  in forEachSystem (system: let
    pkgs = nixpkgs.legacyPackages.${system};

  in rec {
    devShells.default = let
      prompt = "orchestra";
      gcc = pkgs.gcc14;

    in pkgs.mkShell {
      packages = [ gcc ] ++ (with pkgs; [
        ninja
        cmake
        meson
        ccache
        gdb
      ]);

      shellHook = ''
        export PS1="(${prompt}) $PS1"
      '';
    };
  });
}
