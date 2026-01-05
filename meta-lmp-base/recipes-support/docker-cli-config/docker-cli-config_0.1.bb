SUMMARY = "Default system configuration file for Docker cli"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit allarch

SRC_URI = " \
    file://config.json.in \
    file://docker-config.conf.in \
"

S = "${UNPACKDIR}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

FIO_HUB_URL ?= "hub.foundries.io"

do_compile() {
    sed -e 's|@@HUB_URL@@|${FIO_HUB_URL}|g' \
        ${UNPACKDIR}/config.json.in > ${B}/config.json
    sed -e 's|@@LIBDIR@@|${libdir}|g' \
        ${UNPACKDIR}/docker-config.conf.in > ${B}/docker-config.conf
}

do_install() {
        install -Dm 0644 ${B}/config.json ${D}${libdir}/docker/config.json
        install -Dm 0644 ${B}/docker-config.conf ${D}${systemd_system_unitdir}/docker.service.d
}

FILES:${PN} += " \
    ${libdir}/docker \
    ${systemd_system_unitdir} \
"
