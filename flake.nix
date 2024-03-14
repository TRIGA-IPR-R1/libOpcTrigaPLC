{
  description = "libOpcTrigaPLC";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    open26541pp.url = https://github.com/open62541pp/open62541pp?submodules=true;
    open26541pp.flake = false;
    open26541pp.type = "git";
    open26541pp.submodules = true;
  };

  outputs = { self, nixpkgs, flake-utils, open26541pp, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        nativeBuildInputs = with pkgs; [ python3 ];
      in
      {
        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [
            busybox
            cmake
            git
            pkg-config
            stdenv
          ] ++ nativeBuildInputs;
        };

        packages.default = pkgs.stdenv.mkDerivation {
          name = "libOpcTrigaPLC";
          src = ./.;
          cmakeFlags = [
            "-DDONT_INSTALL_CONF=1"
            "-DFETCHCONTENT_SOURCE_DIR_OPEN62541PP=${open26541pp}"
          ];
          nativeBuildInputs = with pkgs; [
            cmake
            pkg-config
          ] ++ nativeBuildInputs;
          buildInputs = with pkgs;[
          ];
        };
      }
    );
}
